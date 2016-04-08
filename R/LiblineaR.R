library(LiblineaR)
library(ROCR)

gen.samples <- function(n, ndim){
    ntrain <- n*0.8
    ntest  <- n - ntrain
    npos <- n/2
    nneg <- n - n/2
    mean.pos <- 1
    sd.pos <- 4
    mean.neg <- -1
    sd.neg <- 4
    X <- matrix(0, nrow=n, ncol=ndim)
    for(i in 1:ndim){
        X[1:npos, i] <- rnorm(npos, mean=mean.pos, sd=sd.pos)
    }
    for(i in 1:ndim){
        X[(npos+1):n, i] <- rnorm(nneg, mean=mean.neg, sd=sd.neg)
    }
    Y <- c(rep('P', npos), rep('N', nneg))
    # shuffle
    index <- sample(1:n)
    X <- X[index,]
    Y <- Y[index]
    return(list(X=X, Y=as.factor(Y)))
}

split.data <- function(X, Y){
    n <- length(Y)
    ntrain <- n*0.8
    ntest <- n - ntrain
    R <- list()
    R$XTrain <- X[1:ntrain,]
    R$YTrain <- Y[1:ntrain]
    R$XTest  <- X[(ntrain+1):n,]
    R$YTest  <- Y[(ntrain+1):n]
    return(R)
}

evaluate <- function(scores, Y) {
    pred <- prediction(scores, Y)
    perf <- performance(pred, 'auc')
    #print(head(Y))
    #print(perf@y.values)
    #print(result$decisionValues)
    #print(result$probabilities)
    #print(data.frame(Score=scores, Y=Y))
    return(perf)

}
train.liblinear <- function(X, Y){
    attach(split.data(X, Y))
    wi <- c(1, 1)
    names(wi) <- c('N', 'P')
    model <- LiblineaR(data=XTrain, target=YTrain, type=7, cost=1, 
                       bias=FALSE, wi=wi, verbose=TRUE)
    result <- predict(model, XTest, proba=TRUE, decisionValues=TRUE)
    scores <- result$probabilities[,1]
    if(YTrain[1] == 'N'){
        scores <- scores
    }
    perf <- evaluate(scores, YTest)
    print(scores)
    return(list(auc=perf@y.values, YTest=YTest, YTrain=YTrain))
}

train.logit <- function(X, Y) {
    attach(split.data(X, Y))
    df <- as.data.frame(XTrain)
    df$Class <- YTrain
    model <- glm(formula = Class ~ ., family=binomial(link='logit'), data=df)
    scores <- predict(model, XTest, type='response')
    evaluate(scores, YTest)
}

datafile <- '/sdat2/KurokawaLab/binbin/tmp/LiblineaR.data'
if(!file.exists(datafile)){
    print('Generating samples')
    data <- gen.samples(500, 100)
    #save(data, file=datafile, compress=FALSE)
} else{
    load(datafile)
}

res <- list()
for(i in 1:10){
    data <- gen.samples(500, 100)
    res[[i]] <- train.liblinear(data$X, data$Y)
}
for(r in res){
    print(r$auc)
}
    
#train.logit(data$X, data$Y)

    
    
    

