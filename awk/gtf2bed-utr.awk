# Two input files are needed: CDS bed (sorted) and transcript exonlist bed (sorted)
# The last column indicate UTR3, UTR5, ncRNA
BEGIN{ FS="\t"; OFS="\t"
    if(ARGC != 3){
        printf "Usage: %s CDS_bed transcript_bed "
        exit 1
    }
}
# read CDS bed file
FNR==NR{
    cno[$4] ++
    cstart[$4, cno[$4]] = $2
    cend[$4, cno[$4]] = $3
}
# read transcript bed file
FNR!=NR{
    if($4 in cno){
        eno = split($7, estarts, ",")
        split($8, eends, ",")
        # UTR5
        u5end = cstart[$4, 1]
        feature = ($6 == "+")? "UTR5" : "UTR3"
        for(i = 1; i <= eno; i ++){
            if(u5end < eends[i]){
                if(estarts[i] < u5end)
                    print $1, estarts[i], u5end, $4, $5, $6, feature
                break
            }
            else{
                print $1, estarts[i], eends[i], $4, $5, $6, feature
            }
        }
        # UTR3
        u3start = cend[$4, cno[$4]]
        feature = ($6 == "+")? "UTR3" : "UTR5"
        for(i = eno; i >= 1; i --){
            if(u3start > estarts[i]){
                if(u3start < eends[i])
                    print $1, u3start, eends[i], $4, $5, $6, feature
                break
            }else{
                print $1, estarts[i], eends[i], $4, $5, $6, feature
            }
        }
    }else{
        print $1, $2, $3, $4, $5, $6, "ncRNA"
    }
}
