import sys, glob

def main():
	do_parse = False
	counts = {}
	for filename in glob.glob('*.txt'):
		print filename
		for line in open(filename):
			if line == '':
				continue
			if '---' in line:
				do_parse = not do_parse
				continue
			if do_parse:
				tokens = line.split()
				print tokens
				count = int(tokens[3])
				name = tokens[-1]
				if name not in counts and '__' not in name:
					counts[name] = [0, 0]
				if '__' not in name:
					counts[name][0] += 1
					counts[name][1] += count

	sorted_dict = sorted(counts.items(), key=lambda x: x[1][0], reverse=True)
	print 'num_calls = ['
	for x,y in sorted_dict:
		print y[0]
	print '];'

	print 'num_call_names = {'
	for x,y in sorted_dict:
		print("'" + x + "';")
	print '};'

	sorted_dict = sorted(counts.items(), key=lambda x: x[1][1], reverse=True)
	print 'raw_counts = ['
	for x,y in sorted_dict:
		print y[1]
	print '];'
	
	print 'raw_count_names={'
	for x,y in sorted_dict:
		print("'" + x + "';")
	print '};'




if __name__ == "__main__":
	main()
    
