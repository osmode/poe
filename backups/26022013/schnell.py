import operator
#program to rapidly extract the essence of a passage

#id_list is a list of lists of id numbers (corresponding to words)
#the x-th list corresponds to the x-th sentence
MAX_RESULTS=1
id_list= []

#read file called "input.txt"
input_in = open("input.txt")
input_content = input_in.read()
input_content_lowercase = input_content.lower()
input_in.close()

input_words = input_content_lowercase.split()

#populate dicionary (keys are words, values are id 
#numbers) with all lowercase input words

#list of meaningless words to exclude
excludes_in = open("excludes")
excludes_content = excludes_in.read()
excludes_in.close()

#create list of common words from file "excludes"
excludes_words = excludes_content.split("\n")
for word in excludes_words:
	word=word.strip()

#loop through excludes_words; if match found
#against input_in, remove that word
for exclude in excludes_words:

	#print "checking word",exclude,"in excludes list"

	padded= ' '+exclude+' '
	padded_eos= ' '+exclude+'.'

	input_content_lowercase=input_content_lowercase.replace(padded,' ')

	input_content_lowercase=input_content_lowercase.replace(padded_eos,'.')


sentences = input_content_lowercase.split(". ")

#for N input sentences, generate N lists of id numbers
num_sentences=len(sentences)
i=0
words_in_sentence=[]
while i < num_sentences:
	words_in_sentence.append([])
	
	i+=1

i=0
while i < num_sentences:

	#print "sentence in sentences: ",sentence,'\n'
	words_in_sentence[i]=sentences[i].split(' ')
	i+=1

#perform cross-over algorithm on every combination of 2 sentences
print "Number of sentences: ",num_sentences

i=0
j=0
#temp lists to hold up to 4 cross-over results
temp_cross1=[]
temp_cross2=[]
temp_cross3=[]
temp_cross4=[]

master_cross_list=[]
allwords=[]

while i < num_sentences:
	while j < num_sentences:

		if j >i:
			for word in words_in_sentence[i]:

				if word in words_in_sentence[j]:
					#print "word match: ",word

					#num words in sentence i and j
					len_i=len(words_in_sentence[i])
					len_j=len(words_in_sentence[j])

					#perform crossover if the matching
					#word is not the first or last
					#in sentence i
					if i != 0 and i != (len_i-1):
						if j > 0:
							temp_cross1.append(words_in_sentence[:i]+words_in_sentence[:(j-1)])
						if j < (len_j -1):
							temp_cross2.append(words_in_sentence[i:]+words_in_sentence[(j+1):])
						if j < (len_j -1):
							temp_cross3.append(words_in_sentence[:i]+words_in_sentence[(j+1):])
						if j > 0:
							temp_cross4.append(words_in_sentence[i:]+words_in_sentence[:(j-1)])
						#if match is found, add temp_lists to master_cross_list 
						#and clear temp lists
						
						if len(temp_cross1) > 0:
							master_cross_list.append(temp_cross1)
							temp_cross1=[]
						if temp_cross2 != []:
							master_cross_list.append(temp_cross2)
							temp_cross2=[]
						if temp_cross3 != []:
							master_cross_list.append(temp_cross3)
							temp_cross3=[]
						if temp_cross4 != []:
							master_cross_list.append(temp_cross4)
							temp_cross4=[]


		j+=1
	j=0
	i+=1

#count create master list (one-dimensional) containing all word occurrences
#in master_list (to apply the set logic algorithm)
for sublist in master_cross_list:
	for word in sublist:
		allwords.append(word)

#now determine which sublist in master_cross_list has the greatest
#overlap with set 'allwords'
num_sublists=len(master_cross_list)
i=0
match_score=0
candidates = { }

while i < num_sublists:
	for word in master_cross_list[i]:
		#for each word in each sublist, cout number of occurences in allwords
		#and sum up totals for all sublists to find logically 'heaviest' sublists
		match_score += allwords.count(word)
			
	#insert match score in dictionary 'candidates'; key= list number in master_cross_list
	#value=count
	candidates[i]=match_score
	#print candidates[i]
	#print match_score
	match_score=0
	i+=1

#sort by descending match score; 'results' is a list of 2-tuples (x,y) where
#x=number of list in 'master_cross-list' and y is its match score
results = sorted(candidates.iteritems(), key=operator.itemgetter(1), reverse=True)
num_results=len(results)
print "Number of results: ",num_results
#print results

i=0
counter=0
output_words=[]

if num_results == 0:
	print "Unable to compress data."

elif len(results) < MAX_RESULTS:
	while i < MAX_RESULTS:
		sublist,score = results[i]
		print master_cross_list[sublist]

		i+=1

else:
	i=0
	print results[:MAX_RESULTS]

	while i < MAX_RESULTS:
		sublist,score = results[i]
		print master_cross_list[sublist]

		i+=1

#consider taking MAX_RESULTS number of lists, removing duplicate words,
#calculating 'hit score' for each word, and returning the top X words
#with the greatest hits

#fix crossover algorithm to account for tail matches

