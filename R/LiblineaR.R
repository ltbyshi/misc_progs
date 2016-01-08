library(LiblineaR)
library(ROCR)

gen.samples <- function(n, ndim){
    ntrain <- n*0.8
    ntest  <- n - ntrain
    npos <- n/2
    nneg <- n - n/2
    mean.pos <- 1
    sd.pos <- runif(ndim, 0.5, 1)
    mean.neg <- -1
    sd.neg <- runif(ndim, 0.5, 1)
    X <- matrix(0, nrow=n, ncol=ndim)
    for(i in 1:ndim){
        X[1:npos, i] <- rnorm(npos, mean=mean.pos, sd=sd.pos)
    }
    for(i in 1:ndim){
        X[(npos+1):n, i] <- rnorm(nneg, mean=mean.neg, sd=sd.neg)
    }
    Y <- c(rep('1', npos), rep('-1', nneg))
    return(list(X=X, Y=Y))
}

train.liblinear <- function(X, Y){
    wi <- c(1, 1)
    names(wi) <- c('1', '-1')
    model <- LiblineaR(data=X, target=Y, type=6, cost=0.1, 
                       bias=FALSE, wi=wi, verbose=TRUE)
    result <- predict(model, X, Y, proba=TRUE, decisionValues=TRUE)
    scores <- result$decisionValues[, 1]
    perf <- performance(prediction(scores, Y), 'auc')
    print(perf)
    #print(head(model$W))
}

datafile <- '/sdat2/KurokawaLab/binbin/tmp/LiblineaR.data'
if(!file.exists(datafile)){
    data <- gen.samples(500, 500000)
    save(data, file=datafile, compress=FALSE)
} else{
    load(datafile)
}
train.liblinear(data$X, data$Y)

    
    
    

