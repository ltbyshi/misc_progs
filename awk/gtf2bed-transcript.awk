# convert GTF to BED format
# extract only exons and set the name field to transcript_id
BEGIN{ FS="\t"; OFS="\t" }
/^#/{ next }
$3 != "transcript" { next }
{
    split($9, a, ";")
    tid = ""
    for(i in a){
        where = match(a[i], /transcript_id "(.+)"/, val)
        if(where != 0){
            tid = val[1]
            break
        }
    }
    if(tid != ""){
        print $1,$4-1,$5,tid,$6,$7
    }
}
