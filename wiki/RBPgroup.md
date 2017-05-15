## Data

## iONMF
* Genomic regions：`/Share/home/lulab/users/liyang/project/RBP/Human/genome/selt_anno` (IBM-C)
* Order of annotation of genomic regions: `/Share/home/lulab/users/liyang/project/RBP/Human/bin/annoLocation.fix.sh` (IBM-C)
* RBP peaks: `/Share/home/lulab/users/liyang/project/RBP/Human/0.data/datasets3/HEK293Dataset/src` (IBM-C)
* Cobinding matrix: `/Share/home/lulab/users/liyang/project/RBP/Human/0.data/datasets3/HEK293Dataset/boolean/HEK293Dataset.boolean.2bind.tmp` (IBM-C)

### Description
iONMF is a variant of non-negative matrix factorization (NMF) that adds a regularization term
to the cost function to increase the orthogonality of the basis matrix. The orthogonal constraint
results in sparse representation of the discovered modules with few overlaps of features between modules.
They also adapted the update equations of the original NMF algorithm to enable prediction of missing features
including response variables in new data with the basis matrix. The iONMF method outperformed other
NMF methods in prediction of RBP binding sites on the test dataset. They also developed a method to
recover the sequence motifs from the k-mer features in the basis matrix and confirmed the discovered motifs
by reported motifs in previous studies.

### Installation
Download from (https://github.com/mstrazar/ionmf).
```bash
cd apps/src/
git clone https://github.com/mstrazar/ionmf
```
For cross-validation, we need to adapt the original NMF algorithm to predict the labels of the test dataset.
This can be done by using Algorithm 2 in [@Strazar2016].

### Prepare data
The script `examples/clip.py` reads dataset in the `datasets/` directory.
The directory structure is like:
```
datasets/
    clip/
        dataset1/
            5000/
                training_sample_0/
                    matrix_Cobinding.tab.gz
                    matrix_GeneOntology.tab.gz
                    matrix_RegionType.tab.gz
                    matrix_Response.tab.gz
                    matrix_RNAkmers.tab.gz
                    matrix_RNAfold.tab.gz
                    motif_fea.gz
                    positions.bedGraph.gz
                    sequences.fa.gz
                test_sample_0/
                    matrix_Cobinding.tab.gz
                    matrix_GeneOntology.tab.gz
                    matrix_RegionType.tab.gz
                    matrix_Response.tab.gz
                    matrix_RNAkmers.tab.gz
                    matrix_RNAfold.tab.gz
                    motif_fea.gz
                    positions.bedGraph.gz
                    sequences.fa.gz
        dataset2/
...
```

Data files:

* `sequences.fa.gz`: FASTA sequences (ATCG), with sequence names in the format: "chrom,strand,start,end; class:0/1".
Each sequence is of fixed length 101nt. The sequences are obtained by extending 50nt from both directions from the cross-linked site.
* `positions.bedGraph.gz`: genomic positions (single nucleotide) of the features, with class in the 4th column.
The file starts with a line 'tract type=bedGraph'.

Feature matrices that can be loaded by the `numpy.loadtxt()` function (include a header at the first line):

* `matrix_RegionType.tab.gz`: 101*5 = 505 columns. Columns are regions. From -50nt to 50nt. Values are indicators (0/1).
Order of regions are: intron, ORF, 5UTR, 3UTR, exon.
The columns form a flatten matrix (position*kmer) with position first: [1, 101], [1+101*1, 99*2], ..., [1+101*4, 101*5].
* `matrix_Response.tab.gz`: only 1 column: class labels (0/1). Column name is 'Class'.
* `matrix_RNAkmers.tab.gz`: 256*99 columns. Columns are kmers (e.g. AAAA, AAAT). Values are occurences of the k-mers (0/1).
K-mers are counted as the occurences start at position i, resulting in (101 - 4 + 1) = 98 k-mers for each sequence.
The order of k-mers are: AAAA, AAAC, AAAG, AAAT, AACA, ...
The columns actually form a flatten matrix (position*kmer) with position first: [1, 99], [1+99*1, 99*2], ..., [1+99*255, 99*256].
* `matrix_RNAfold.tab.gz`: 101 columns. Column names are all 'dsRNA'. Values are base pair probability (0-1).
* `matrix_Cobinding.tab.gz`: 30 columns. Columns are names of other RBP datasets. Values are indicators of cobinding (0/1).
* `motif_fea.gz`: 103 columns. Columns are motifs discovered from RNAcompete data (in the `pwms/` directory).

### Generate the feature matrices from CLIP-seq peaks
Set environment variable:
```bash
dataset_dir=datasets
```
1. Get the positions of the peaks as a bed file (peaks.bed).
```bash
awk 'BEGIN{OFS="\t"}{$4=$4 "_" NR;print $1,$2,$3,$4,$5,$6}' \
    ~/data/CLIPdb/3_merge_nt/ELAVL1_merge.bed > $dataset_dir/peaks.bed
```
Show the first few lines of `peaks.bed`:
```
chr1	20120	20140	ELAVL1_1	20	-
chr1	265160	265180	ELAVL1_2	20	-
chr1	569040	569060	ELAVL1_3	20	-
chr1	569380	569400	ELAVL1_4	20	-
chr1	637720	637740	ELAVL1_5	20	-
```
2. Set the nucleotide in the middle of each interval as the cross-linked site.
```bash
awk 'BEGIN{OFS="\t"}{$2=int(($2+$3-1)/2);$3=$2+1;print}' $dataset_dir/peaks.bed > $dataset_dir/positions.bed
```
Show the first few lines of `positions.bed`:
```
chr1	20079	20180	ELAVL1_1	20	-
chr1	265119	265220	ELAVL1_2	20	-
chr1	568999	569100	ELAVL1_3	20	-
chr1	569339	569440	ELAVL1_4	20	-
chr1	637679	637780	ELAVL1_5	20	-
```
3. Extend the cross-linked sites in both directions for 50nt.
Requires a FASTA index (created by `samtools faidx`) for the genomic sequences.
Removes windows that does not fall completely inside the chromosomes.
```bash
awk 'BEGIN{OFS="\t"}FNR==NR{l[$1]=$2} FNR!=NR{if(($2<50)||($3+50>l[$1])) next; $2-=50;$3+=50;print}' \
    ~/data/genomes/fasta/Human/hg19.fa.fai $dataset_dir/positions.bed > $dataset_dir/windows.bed
```
Show the first few lines of `windows.bed`:
```
chr1	20079	20180	ELAVL1	20	-	4.13829
chr1	265119	265220	ELAVL1	20	-	4.66847
chr1	568999	569100	ELAVL1	20	-	4.13027
chr1	569339	569440	ELAVL1	20	-	4.231105
chr1	637679	637780	ELAVL1	20	-	4.17849
```
4. Extract sequences using `windows.bed` from the genome sequences (hg19)
```bash
bedtools getfasta -s -fi ~/data/genomes/fasta/Human/hg19.fa -bed $dataset_dir/windows.bed \
    | awk '/^>/{ match($0, ">([^:]+):([0-9]+)-([0-9]+)[(]([+-])[)]", a);
        printf ">%s,%s,%s,%s; class:1", a[1],a[4],a[2],a[3]; printf "\n";next}{print}'> $dataset_dir/sequences.fa
gzip -c $dataset_dir/sequences.fa > sequences.fa.gz
```

5. Calculate k-mer occurences
```bash
./matrix_RNAkmers sequences.fa > $dataset_dir/matrix_RNAkmers.tab
awk 'NR==1{for(i=1;i<=NF;i++)a[i]=$i;next} NR==2{for(i=1;i<=NF;i++) if($i>0) print (i-1)%99+1, a[i]}' \
    $dataset_dir/matrix_RNAkmers.tab | sort -k1,1n

gzip $dataset_dir/matrix_RNAkmers.tab

cp -r ~/apps/src/ionmf/datasets/clip/10_PARCLIP_ELAVL1A_hg19/5000/training_sample_0 ref_dataset
```

6. Calculate RNAfold features
```bash
./matrix_RNAfold < $dataset_dir/sequences.fa > $dataset_dir/matrix_RNAfold.tab
gzip $dataset_dir/matrix_RNAfold.tab
```
**Issue**
* The calculated base pair probabilities did not correlate well with the published dataset.

7. Get region types
```bash
for region in CDS 5UTR 3UTR exon transcript;do
    ~/projects/tests/python/read_gtf.py --format bed --feature $region \
        -i ~/data/gtf/gencode.v19/gencode.v19.annotation.gtf \
        -o ~/data/gtf/gencode.v19/regions/${region}.bed
done
bedtools subtract -s -a ~/data/gtf/gencode.v19/regions/transcript.bed \
    -b ~/data/gtf/gencode.v19/regions/exon.bed \
    | bedtools sort > ~/data/gtf/gencode.v19/regions/intron.bed

annotation_dir=~/data/gtf/gencode.v19/regions
for region in intron CDS 5UTR 3UTR exon;do
    bedtools intersect -s -a $dataset_dir/windows.bed -b $annotation_dir/${region}.bed -wo > $dataset_dir/region.${region}.bed
done
./matrix_RegionType.py $dataset_dir > $dataset_dir/matrix_RegionType.tab
gzip $dataset_dir/matrix_RegionType.tab
```
Convert FASTA to bed:
```bash
awk 'BEGIN{OFS="\t"} /^>/{ split($2, a, ","); n++; print a[1],a[3],substr(a[4], 0, length(a[4]) -1),"seq_" n,"0",a[2]}' ref_dataset/sequences.fa > ref_dataset/windows.bed
```

### Train the iONMF model
The features used can be modified by changing the script `examples/clip.py` (Line 66):
```python
training_data = load_data("../datasets/clip/%s/5000/training_sample_0" % protein,
    go=False, kmer=False)
```
The definition of function `load_data` is:
```python
def load_data(path, kmer=True, rg=True, clip=True, rna=True, go=True):
```
By running `python clip.py $dataset`, it trains a model and predict the test dataset.
The AUC is reported and a plot is generated (need to open a window).

```bash
export PYTHONPATH=~/sshfs/ibme/apps/src/ionmf
mkdir Jobs
{
for protein in $(ls ~/apps/src/ionmf-full/datasets/clip);do
    data_dir=~/apps/src/ionmf-full/datasets/clip/$protein/30000/training_sample_0
    for alpha in -3 -2 -1 0 1 2 3;do
    if [ -d "$data_dir" ];then
        echo "PYTHONPATH=~/apps/src/ionmf python bin/clip.py train --protein $protein --data-dir $data_dir --model-file models/${protein}/${alpha}.pkl --alpha $alpha"
    fi
    done
done
} > Jobs/ionmf.train.txt
qsubgen -n ionmf.train -q Z-LU -a 1-40 -j 2 --bsub --task-file Jobs/ionmf.train.txt
```

```bash
{
for protein in $(ls ~/apps/src/ionmf-full/datasets/clip);do
    data_dir=~/apps/src/ionmf-full/datasets/clip/$protein/30000/test_sample_0
    for alpha in -3 -2 -1 0 1 2 3;do
    if [ -d "$data_dir" ];then
        echo "PYTHONPATH=~/apps/src/ionmf bin/clip.py test --protein $protein --data-dir $data_dir --model-file models/${protein}/${alpha}.pkl --pred-file predictions/${protein}/${alpha}.h5"
    fi
    done
done
} > Jobs/ionmf.test.txt
qsubgen -n ionmf.test -q Z-LU -a 1-25 -j 4 --bsub --task-file Jobs/ionmf.test.txt
```

## iDeep
### Description
iDeep is a method for predicting RBP binding sites and motifs using a hybrid deep learning model
that combines convolution neural networks (CNN) and deep belief networks (DBN). iDeep trains separate neural networks
on multiple types of features (genomic region type, RBP co-binding, RNA secondary structure, motif occurences) and
merge them into a single neural network. The integrative model can also recovers sequence motifs that are consist with
motifs reported in previous studies.

### Installation
Download iDeep from Github:
```bash
cd apps/src
git clone https://github.com/xypan1232/iDeep.git
```

### Prepare data
The dataset format is the same as iONMF[https://github.com/mstrazar/ionmf].

Training data directory: `datasets/clip/$name/5000/training_sample_0`,
test data directory: `datasets/clip/$name/5000/test_sample_0`.


Most files are the same as the files used by iONMF except that `matrix_GeneOntology.tab.gz` is not used and an additional feature matrix
`motif_fea.gz` is needed by iDeep.

Generate motif feature table.

Edit the file `get_RNA_motif_fea.sh` to change the parameters. This script reads sequences from a FASTA file (sequences.fa)
and scan all the motifs in the `pwms/` directory. The result motif table is saved to `motif_fea.gz` file. in the dataset directory.
The script also need a feature order file that corresponds to file names in the `pwms` directory.

### Train the models
Run
```bash
python ideep.py
```
The script scans all datasets in the datasets directory.

```bash
export IDEEP_PATH=~/apps/src/iDeep
for protein in $(ls ~/apps/src/iDeep/datasets/clip);do
    data_dir=ref_dataset
    python bin/make_feature_table.py -f <(zcat $data_dir/sequences.fa.gz) -o <(ls $IDEEP_PATH/pwms) -m $IDEEP_PATH/pwms/ -s $data_dir/motif_fea.gz -c $IDEEP_PATH/cbust_folder/cbust
done

mkdir Jobs
{
for protein in $(ls ~/apps/src/iDeep/datasets/clip);do
    data_dir=~/apps/src/iDeep/datasets/clip/$protein/5000/training_sample_0
    for alpha in -3 -2 -1 0 1 2 3;do
    if [ -d "$data_dir" ];then
        echo "OMP_NUM_THREADS=4 python bin/ideep.py train --protein $protein --data-dir $data_dir --model-file models/${protein}.h5"
    fi
    done
done
} > Jobs/ideep.train.txt
qsubgen -n ideep.train -q Z-LU -a 1-25 -j 4 --bsub --task-file Jobs/ideep.train.txt

{
for protein in $(ls ~/apps/src/iDeep/datasets/clip);do
    data_dir=~/apps/src/iDeep/datasets/clip/$protein/5000/test_sample_0
    for alpha in -3 -2 -1 0 1 2 3;do
    if [ -d "$data_dir" ];then
        echo "OMP_NUM_THREADS=4 python bin/ideep.py test --protein $protein --data-dir $data_dir --model-file models/${protein}.h5 --pred-file predictions/${protein}.h5"
    fi
    done
done
} > Jobs/ideep.test.txt
qsubgen -n ideep.test -q Z-LU -a 1-25 -j 4 --bsub --task-file Jobs/ideep.test.txt

python bin/report.py test_auc > reports/test_auc.txt

```
## GraphProt

### Description
GraphProt is a method for sequence and secondary structure patterns of RBP binding sites.
GraphProt models abstract shapes of RNA secondary structure as graphs and encodes the
sequence and structures as a graph kernel. Then it trains a SVM classifier to predict RBP-binding
sites on CLIP-seq data. The GraphProt method outperformed RNAcontext and matrixREDUCE in terms of AUROC and
also recoverd sequence and structure motifs from the trained models.

### Installation
Download RNAshapes 2.1.6 (http://bibiserv.techfak.uni-bielefeld.de/download/tools/rnashapes.html) and because GraphProt is not compatible with RNAshapes 3.x.
```bash
tar zxf RNAshapes-2.1.6.tar.gz
cd RNAshapes-2.1.6/
./configure --prefix=$prefix
make
make install
```
Add `$prefix/bin` to the `$PATH` environment variable.

Download perf from (http://osmot.cs.cornell.edu/kddcup/software.html) and install. Add perf to the `$PATH` variable.

Download GraphProt 1.1.4 from: (http://www.bioinf.uni-freiburg.de/Software/GraphProt/).
```bash
wget 'http://www.bioinf.uni-freiburg.de/Software/GraphProt/GraphProt-1.1.4.tar.bz2'
tar jxf GraphProt-1.1.4.tar.bz2
```
Add `GraphProt-1.1.4` to the `$PATH` environment variable.

### Prepare the dataset
* Extract RNA sequences around each RBP-binding site (upstream and downstream 150nt) to a FASTA file.
* Mark the binding region in each RNA sequence as a viewpoint in **uppercase** letters and other bases in **lowercase** letters.
* Split each FASTA file into a training set (`train_positives.fa`) and a test set (`test_positives.fa`).
* Sample an equal number of binding sites to the to a positive dataset as a negative dataset (`test_positives.fa` and `test_negatives.fa`).

### Optimize hyper-parameters
```bash
GraphProt.pl -mode classification --action ls -fasta train_positives.fa -negfasta train_negatives.fa
```
### Train the model
```bash
GraphProt.pl -mode classification --action train -fasta train_positives.fa -negfasta train_negatives.fa
```
The GraphProt model is saved to a file named `GraphProt.model`.

### Predict
```bash
Graphprot.pl --action predict -mode classification -model GraphProt.model -fasta test_positives.fa -negfasta test_negatives.fa
```
### Cross-validation (10 fold)
```bash
GraphProt.pl -mode classification --action cv -fasta test_positives.fa -negfasta test_negatives.fa
```

## Response letter

### Reviewer 2, Comment 1
The three methods mentioned (PMIDs 24451197, 26787667, 28245811) were developed to predict RBP binding sites on RNAs in different approaches.
iDeep is a method for predicting RBP binding sites and motifs using a hybrid deep learning model
that combines convolution neural networks (CNN) and deep belief networks (DBN).
iONMF is a variant of non-negative matrix factorization (NMF) that adds a regularization term
to the cost function to increase the orthogonality of the basis matrix. It incorporates multiple sources of
features including k-mer occurences, co-binding by other RBPs, RNA secondary structure, etc. and also adapted the
original NMF method to predict cross-linked sites.
GraphProt encodes sequence and secondary structure into a graph kernel and predicts RBP binding sites from the patterns learned
from CLIP-seq data.
However, the goal of our method is not to predict binding sites of individual RBPs, but to identify groups of RBPs that bind to
similar RNA targets and their shared sequence patterns (motifs). We identified the RBP groups based on co-binding patterns of RBPs
rather than the sequence or structure patterns.
The method that is most related to our work among the three mentioned methods is iONMF, which uses a regularized NMF algorithm to
cluster the features into interpretable modules. Generally, NMF is used as a dimension reduction technique and is not used for classification tasks.
The authors of iONMF adapted NMF to allow for prediction of missing features from the basis matrix learned from training data.
We could adapt our method to allow for prediction of RBP-binding sites, but that is not the major goal of our method.
RBP co-binding is one group of features and they found that k-mer and structure were
most contributing features to prediction of RBP binding sites.
The feature matrix in our method also differs from iONMF in that we used normalized counts while iONMF used binary values.
Transformation of read counts into binary values results in loss of binding strength information of the binding sites, which is considered in our method.
They also identified most associated RBPs for each individual RBP and
clustered the binding motifs of the RBPs. However, they discovered the motifs from the k-mer features of all binding sites for each individual RBP rather
than from RBP co-binding sites.
In summary, our method is different from the three methods in:
1. Normalization of CLIP-seq counts
2. Identify groups of RBPs that bind similar RNA targets and discovered the shared motifs.


# References
[@Strazar2016] Stražar, M., Žitnik, M., Zupan, B., Ule, J., and Curk, T. (2016). Orthogonal matrix factorization enables integrative analysis of multiple RNA binding proteins. Bioinformatics 32, 1527–1535.
