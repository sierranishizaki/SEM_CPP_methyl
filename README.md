# SEMplMe
perl implementation of the SEMplMe algorithm

# Installation of SEMplMe
Clone a copy of the SEMplMe repository and submodules:

```
git clone --recurse-submodules https://github.com/Boyle-Lab/SEMplMe.git
```

Build external libraries:
```
cd SEMplMe/lib/libBigWig
make
cd ..
make
mv */*.so .
cd ..
```

Symlink to bowtie index location (use your own index location):
```
ln -s /data/genomes/hg38/bowtie_index/ data
```

Build SEMpl
```
make
```
 
# Usage information
SEMplMe functions on SEMpl output, please run SEMpl before attempting to use SEMplMe outside of the demo. Example:

```
./iterativeSEM -PWM examples/MA0114.1.pwm -merge_file examples/wgEncodeOpenChromDnaseHepg2Pk.narrowPeak -big_wig examples/wgEncodeHaibTfbsHepg2Hnf4asc8987V0416101RawRep1.bigWig -TF_name HNF4A -genome data/hg19 -output results/HNF4A
```

For more information on SEMpl please go to  https://github.com/Boyle-Lab/SEMpl


# SEMplMe Demo 
SEMplMe requires whole genome bisulfite sequencing (WGBS) data, which can be downloaded from ENCODE. Of note, SEMplMe uses WGBS data in .wig format. Preconverted .bigwig to .wig files are available for download from dropbox. The following example will build the SEM with methylation for HNF4a in HepG2 cells given the example data including a precomputed example SEMpl output
```
cd examples

wget "https://www.dropbox.com/s/ila7tq11w6o7nke/ENCFF073DUG.wig.tar.gz"

tar xvfz ENCFF073DUG.wig.tar.gz

cd ..

perl ./generateSignalMethylTable.pl --TF_name HNF4A --WGBS examples/ENCFF073DUG.wig
```

# Expected SEMplMe output

We include a small demo of SEMplMe for HNF4A in HepG2 cells. The expected output is:
```
Integrating methylation...Done
Creating SEM...Done
Creating R plot................................................................\
.............................................Done
```