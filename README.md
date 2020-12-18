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

tar -xvf ENCFF073DUG.wig.tar.gz

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

# WGBS datasets

Available in .wig format for download from dropbox
```
HepG2:
wget "https://www.dropbox.com/s/ila7tq11w6o7nke/ENCFF073DUG.wig.tar.gz"

K562:
wget "https://www.dropbox.com/s/lc8ltup91j1jb8j/ENCFF872YSC.wig.tar.gz"

GM12878:
wget "https://www.dropbox.com/s/zhqk6xyxoe0kqa1/ENCFF796NFQ.wig.tar.gz"

IMR-90:
wget "https://www.dropbox.com/s/fjd9hgqeo1pdjj9/ENCFF433WIE.wig.tar.gz"

H1-hESC:
wget "https://www.dropbox.com/s/wlhs8omjn70ajh7/ENCFF770YJW.wig.tar.gz"

GM23248:
wget "https://www.dropbox.com/s/k8ir44dn21e1lgo/ENCFF390POK.wig.tar.gz"

```