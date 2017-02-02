install.packages("slam")
install.packages("tm")
install.packages("wordcloud")

library(tm)
library(wordcloud)

mycorpus <- Corpus(VectorSource(all_data$message))

corpus_clean <- tm_map(mycorpus, tolower)
corpus_clean <- tm_map(mycorpus, removeNumbers)

corpus_clean2 <- tm_map(corpus_clean, removeWords, stopwords('english'))
corpus_clean3 <- tm_map(corpus_clean2, removePunctuation)
corpus_clean <- tm_map(corpus_clean, stripWhitespace)
#corpus_clean <- tm_map(corpus_clean, trim)

wordcloud(corpus_clean2, scale=c(5,0.5), max.words=100, random.order=FALSE, rot.per=0.35, use.r.layout=FALSE, colors=brewer.pal(8, 'Dark2'))
