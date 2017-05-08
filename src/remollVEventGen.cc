#include "remollVEventGen.hh"

#include "G4RotationMatrix.hh"
#include "G4GenericMessenger.hh"

#include "remollBeamTarget.hh"
#include "remollVertex.hh"
#include "remollEvent.hh"
#include "remollRun.hh"
#include "remollRunData.hh"

remollVEventGen::remollVEventGen()
: fThCoM_min(0.0), fThCoM_max(0.0), fTh_min(0.0), fTh_max(0.0),
  fPh_min(0.0), fPh_max(0.0), fE_min(0.0), fE_max(0.0) {

    fBeamTarg = remollBeamTarget::GetBeamTarget();
    fRunData  = remollRun::GetRunData();

    fSampType       = kCryogen;
    fApplyMultScatt = false;
    // Set initial number of particles and create particle gun
    SetNumberOfParticles(fNumberOfParticles);

    // Create generic messenger
    fMessenger = new G4GenericMessenger(this,"/remoll/","Remoll properties");
    fMessenger->DeclarePropertyWithUnit("emax","GeV",fE_max,"Maximum generation energy");
    fMessenger->DeclarePropertyWithUnit("emin","GeV",fE_min,"Minimum generation energy");
    fMessenger->DeclarePropertyWithUnit("thcommax","deg",fThCoM_max,"Maximum CoM generation theta angle");
    fMessenger->DeclarePropertyWithUnit("thcommin","deg",fThCoM_min,"Minimum CoM generation theta angle");
    fMessenger->DeclarePropertyWithUnit("thmax","deg",fTh_max,"Maximum generation theta angle");
    fMessenger->DeclarePropertyWithUnit("thmin","deg",fTh_min,"Minimum generation theta angle");
    fMessenger->DeclarePropertyWithUnit("phmax","deg",fPh_max,"Maximum generation phi angle");
    fMessenger->DeclarePropertyWithUnit("phmin","deg",fPh_min,"Minimum generation phi angle");
}

remollVEventGen::~remollVEventGen()
{
    delete fMessenger;
}

void remollVEventGen::SetNumberOfParticles(G4int n)
{
  // Store new number of particles
  fNumberOfParticles = n;

  // Delete old particle gun
  if (fParticleGun) {
    delete fParticleGun;
    fParticleGun = 0;
  }
  // Create new particle gun
  fParticleGun = new G4ParticleGun(fNumberOfParticles);
}

remollEvent* remollVEventGen::GenerateEvent()
{
    // Set up beam/target vertex
    remollVertex vert   = fBeamTarg->SampleVertex(fSampType);

    /////////////////////////////////////////////////////////////////////
    // Create and initialize values for event
    remollEvent *thisev = new remollEvent();
    thisev->fVertexPos    = fBeamTarg->fVer;
    if( fApplyMultScatt ) {
        thisev->fBeamMomentum = fBeamTarg->fSampE*(fBeamTarg->fDir.unit());
    } else {
        thisev->fBeamMomentum = fBeamTarg->fSampE*G4ThreeVector(0.0, 0.0, 1.0);
    }
    /////////////////////////////////////////////////////////////////////

    SamplePhysics(&vert, thisev);

    PolishEvent(thisev);

    return thisev;
}


void remollVEventGen::PolishEvent(remollEvent *ev) {
    /*!
       Here it's our job to:
          Make sure the event is sane
          Apply multiple scattering effects to the final
        products if applicable
      Calculate rates from our given luminosity
      Calculate measured asymmetry from polarization
      Calculate vertex offsets
     */

    if( !ev->EventIsSane() ) {
        G4cerr << __FILE__ << " line " << __LINE__ << ":  Event check failed for generator " << fName << ".  Aborting" << G4endl;
        ev->Print();
        exit(1);
    }

    G4ThreeVector rotax      = (fBeamTarg->fDir.cross(G4ThreeVector(0.0, 0.0, 1.0))).unit();
    G4RotationMatrix msrot;
    msrot.rotate(fBeamTarg->fDir.theta(), rotax);

    std::vector<G4ThreeVector>::iterator iter;

    if( fApplyMultScatt ) {
        for( iter = ev->fPartRealMom.begin(); iter != ev->fPartRealMom.end(); iter++ ) {
            //  rotate direction vectors based on multiple scattering
            (*iter) *= msrot;
        }

        // Rotate position offsets due to multiple scattering
        for( iter = ev->fPartPos.begin(); iter != ev->fPartPos.end(); iter++ ) {
            //  rotate direction vectors based on multiple scattering
            (*iter) *= msrot;
        }
    }

    // Add base vertex
    for( iter = ev->fPartPos.begin(); iter != ev->fPartPos.end(); iter++ ) {
        (*iter) += ev->fVertexPos;
    }
    

    if ( ev->fRate == 0 ){// If the rate is set to 0 then calculate it using the cross section
    	ev->fRate  = ev->fEffXs*fBeamTarg->GetEffLumin()/((G4double) fRunData->GetNthrown());
    }
    else { // For LUND - calculate rate and cross section	
    	ev->fEffXs = ev->fRate*((G4double) fRunData->GetNthrown())/(fBeamTarg->GetEffLumin());
    	ev->fRate = ev->fRate/((G4double) fRunData->GetNthrown());
    }

    ev->fmAsym = ev->fAsym*fBeamTarg->fBeamPol;
}

