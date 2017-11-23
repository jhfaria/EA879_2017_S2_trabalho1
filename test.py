#!/usr/bin/env python2

import os

files = os.listdir("tests")

numero_imagem = 1

for test in files:
	log = os.popen("./main < tests/{:s}".format(test)).read()

	print "*** Imagem {:d} ***".format(numero_imagem)
	
	numero_imagem += 1

	lines = log.split("\n")

	for line in lines:
		words = line.split(" ")

		if(words[0] == "Tempo"):
			print line

os.system("mv novaI* novas_imagens/")

