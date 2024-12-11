#!/bin/bash
#SBATCH --account=halla
#SBATCH --partition=production
#SBATCH --job-name=Det78_1AcShld
#SBATCH --output=/farm_out/%u/%x-%j-%N.out
#SBATCH --error=/farm_out/%u/%x-%j-%N.err
#SBATCH --time=20:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=10G
tcsh -c "source /apps/root/6.18.00/setroot_CUE"
cd /w/halla-scshelf2102/moller12gev/chandan/sim/remoll 
echo "Current working directory is `pwd`"
./build/remoll tid_det78_1.mac
