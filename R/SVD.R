printVar <- function(x, name=deparse(substitute(x))){
    cat(paste(name, "=", "\n"));
    print(x);
}

set.seed(1763);
X <- matrix(rnorm(6*4), nrow=6, ncol=4);
printVar(X);
centers <- apply(X, 2, sum)/nrow(X);
printVar(centers);
Xs <- sweep(X, 2, centers);
printVar(Xs);
res.svd <- svd(Xs);
printVar(res.svd$d);
res.pca <- prcomp(X);
printVar(res.pca$sdev);
