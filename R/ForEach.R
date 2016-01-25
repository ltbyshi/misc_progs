library(doParallel)
library(foreach)

cl <- makeCluster(4)
registerDoParallel(cl)
res <- foreach(a=1:4, .combine='c') %:% foreach(b=1:3, .combine='c') %dopar% {
    Sys.sleep(1)
    a*3 + b
}
print(res)
