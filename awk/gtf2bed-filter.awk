# convert GTF to BED format
# extract only exons and set the name field to transcript_id
# Variables:
#   feature: only keep a specific feature (default is all)
#   attribute: set the attribute as name (default is gene_id)
BEGIN{ FS="\t"; OFS="\t";
   if(attribute == ""){
       attr_pat = "transcript_id \"(.+)\""
   }else{
       attr_pat = attribute " \"(.+)\""
   }
}
/^#/{ next }
{
    if($0 ~ /^#/) 
        next
    if((feature != "") && ($3 != feature))
        next
    split($9, a, ";")
    tid = ""
    for(i in a){
        where = match(a[i], attr_pat, val)
        if(where != 0){
            tid = val[1]
            break
        }
    }
    if(tid != ""){
        if(feature != "")
            print $1,$4-1,$5,tid,$6,$7,feature
        else
            print $1,$4-1,$5,tid,$6,$7
    }
}
