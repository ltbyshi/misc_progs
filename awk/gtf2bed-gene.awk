# convert GTF to BED format
# extract only exons and set the name field to transcript_id
# Variables:
#   feature: only keep a specific feature (default is all)
#   attribute: set the attribute as name (default is gene_id)
BEGIN{ FS="\t"; OFS="\t";
}
/^#/{ next }
{
    if($0 ~ /^#/) 
        next
    if($3 != "gene")
        next
    where = match($9, /gene_id "([^"]+)"/, val)
    if(where == 0) next
    gid = val[1]
    where = match($9, /gene_type "([^"]+)/, val)
    if(where == 0) next
    gene_type = val[1]
    if(gid != ""){
        print $1,$4-1,$5,gid,$6,$7,gene_type
    }
}
