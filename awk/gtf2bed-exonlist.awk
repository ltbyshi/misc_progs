# Read exon bed file (sorted) and get intron regions
# Output is a bed file
# Column 2 and 3 are start and end positions of the transcript
# Column 7 and 8 are comma-separated list of start and end positinos of the exons
BEGIN{ FS="\t"; OFS="\t" }
{
    chr[$4] = $1
    eno[$4] ++
    estart[$4, eno[$4]] = $2
    eend[$4, eno[$4]] = $3
    strand[$4] = $6
}
END{
    for(name in chr){
        starts = ""
        ends = ""
        for(i = 1; i <= eno[name]; i ++){
            starts = starts estart[name, i] ","
            ends = ends eend[name, i] ","
        }
        starts = substr(starts, 1, length(starts) - 1)
        ends = substr(ends, 1, length(ends) - 1)
        print chr[name], estart[name, 1], eend[name, eno[name]], name, ".", strand[name], starts, ends
    }
}
