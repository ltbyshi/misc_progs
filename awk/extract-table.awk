# read header
BEGIN{
    if(length(colsep) == 0){
        colsep = ","
    }
    # get the the columns to be selected
    n_sel = split(cols, _cols, colsep)
    OFS=FS
}
# read header line
NR==1{
    for(i = 1; i <= NF; i ++){
        for(j = 1; j <= n_sel; j ++){
            if($i == _cols[j]){
                ignore[i] = 1
                break
            }
        }
    }
}
NR>1{
    for(i = 1; i <= NF; i ++){
        if(i in ignore)
            $i = ""
    }
    print
}   
