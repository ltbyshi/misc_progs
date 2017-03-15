# Read a gtf file (sorted by chromosome and start position)
# Output transcripts in BED12 format where Column 10-12 contains the exon positions
# The output file is useful for extracting mature transcript seqeunces from genome FASTA file
function get_tid(s)
{
  split(s, a, ";")
  tid = ""
  for(i in a){
      where = match(a[i], /transcript_id "(.+)"/, val)
      if(where != 0){
          tid = val[1]
          break
      }
  }
  return tid
}

BEGIN{ FS="\t"; OFS="\t"}
/^#/ {next}
$3 == "transcript" {
  tid = get_tid($9)
  ts_chr[tid] = $1
  ts_start[tid] = $4 - 1
  ts_end[tid] = $5
  ts_strand[tid] = $7
  ts_num ++
  tids[ts_num] = tid
}
$3 == "exon" {
  tid = get_tid($9)
  ex_num[tid] ++
  # get a list of exon lengths and start positions
  ex_start[tid, ex_num[tid]] = $4 - 1
  ex_size[tid, ex_num[tid]] = $5 - $4 + 1
}
END {
  for(i in tids){
    tid = tids[i]
    block_starts = ""
    block_sizes = ""
    for(j = 1; j <= ex_num[tid]; j ++){
      block_starts = block_starts (ex_start[tid, j]-ts_start[tid]) ","
      block_sizes = block_sizes ex_size[tid, j] ","
    }
    block_starts = substr(block_starts, 1, length(block_starts) - 1)
    block_sizes = substr(block_sizes, 1, length(block_sizes) - 1)
    print ts_chr[tid], ts_start[tid], ts_end[tid], tid, "0", ts_strand[tid], "0", "0", "0", ex_num[tid], block_sizes, block_starts
  }
}
