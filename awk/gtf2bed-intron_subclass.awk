# Input is the output of gtf2bed-exonlist.awk
# The intervals between exons are defined as introns
# The intron regions are diviced into subclasses:
# ${num}: 15, 50, 100, 150
# larger ${num} is assigned with higher precedence
#   intron_5p_${num}: 1-${num} from the 5'-end of the intron
#   intron_3p_${num}: 1-${num} from the 5'-end of the intron
#   intron_middle: ${num}-(length-${num})
BEGIN { FS="\t"; OFS="\t"; nl=split("0,150", l, ",") }
{
    eno = split($7, estarts, ",")
    split($8, eends, ",")
    for(e = 1; e < eno; e ++){
        istart = eends[e]
        iend = estarts[e + 1]
        print $1,istart,iend,$4,$5,$6,"intron"
        prime = ($6 == "+")? "5p" : "3p"
        for(i = 2; i <= nl; i ++)
            print $1,istart+l[i-1],istart+l[i],$4,$5,$6,"intron_"prime"_"l[i]
        print $1,istart+l[nl],iend-l[nl],$4,$5,$6,"intron_middle"
        prime = ($6 == "+")? "3p" : "5p"
        for(i = nl; i >= 2; i --)
            print $1,iend-l[i],iend-l[i-1],$4,$5,$6,"intron_"prime"_"l[i]
    }
}
