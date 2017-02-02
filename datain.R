#set working dir
dir <- getwd()
dirname <- paste(dir, "/data/cleanData", sep="")

setwd(dirname)

# read CSV file into data frame
hangout <- read.csv("hangout.csv", header = TRUE, stringsAsFactors=FALSE)
wp1 <- read.csv("whatsapp_2014.csv", header = TRUE, stringsAsFactors=FALSE)
wp2 <- read.csv("whatsapp_2015.csv", header = TRUE, stringsAsFactors=FALSE)

all_data <- rbind(hangout, wp1)
all_data <- rbind(all_data, wp2)
