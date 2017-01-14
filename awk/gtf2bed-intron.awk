# Input is the output of gtf2bed-exonlist.awk
# The intervals between exons are defined as introns
BEGIN { FS="\t"; OFS="\t"; }
{
    eno = split($7, estarts, ",")
    split($8, eends, ",")
    for(i = 1; i < eno; i ++){
        print $1, eends[i], estarts[i + 1], $4, $5, $6, "intron"
    }
}
