import argparse
import os


parser = argparse.ArgumentParser()

parser.add_argument("--input_corpus",help="Corpus to train model with",required=True,type=str)
parser.add_argument("--output_dir",help="dir name for output files",required=True,type=str)
parser.add_argument("--strategy",help="strategy to train model",choices=["rouge", "semantic", "shuffling","MSR","rouge_duplicate","greedy_rouge"],required=True,type=str)
parser.add_argument("--masking_ratio",default=0.8,help="Masking ratio for selected sentences",type=float)
parser.add_argument("--MSR_ratio",default=0.9,help="MSR ratio from paper. MSR_ratio + masking_ration should be 1 if MSR is selected",type=float)
parser.add_argument("--SH_ratio",default=0.8,help="rate of sentence corruption",type=float)
parser.add_argument("--max_shuffling_length_ratio",default=0.4,help="maximum length of spans to be shuffled. this ratio is depends on sentences length",type=float)
parser.add_argument("--use_single_mask",default=True,help="use single mask for adjacent sentences",type=bool)
parser.add_argument("--compression_rate",default=0.3,help="rate of selecting top sentences",type=float)
parser.add_argument("--bulk_size",default=1000,help="number of documents to load in memory at the same time",type=int)
args = parser.parse_args()

strategy = args.strategy

if strategy == "rouge" or strategy == "greedy_rouge" or strategy == "rouge_duplicate" or strategy == "semantic":
    r1 = args.masking_ratio
    r2 = r1
    r3 = 1.01
    r4 = 1.01
    single_mask = args.use_single_mask
    reorder_sentence = False
elif strategy == "shuffling":
    strategy = "semantic"
    r1 = args.masking_ratio
    r2 = r1
    r3 = 1.01
    r4 = 1.01
    single_mask = args.use_single_mask
    reorder_sentence = True
    pass
elif strategy == "MSR":
    strategy = "semantic_with_reordering"
    r1 = 1.01
    r2 = r1
    r3 = 1.01
    r4 = 1.01
    single_mask = args.use_single_mask
    reorder_sentence = False

os.system("make")
os.system("./run.out {input} {output} {type} {r1} {r2} {r3} {r4} {single_mask} {reorder_sentence} {masked_sentence_ratio} {shuffle_length_ration} {MSR_rate} {SH_rate} {input_size}".format(
    input = args.input_corpus,
    output = args.output_dir,
    type = strategy,
    r1 = r1,
    r2 = r2,
    r3 = r3,
    r4 = r4,
    single_mask = single_mask,
    reorder_sentence = reorder_sentence,
    masked_sentence_ratio = args.compression_rate,
    shuffle_length_ration = args.max_shuffling_length_ratio,
    MSR_rate = args.MSR_ratio,
    SH_rate = args.SH_ratio,
    input_size = args.bulk_size
))
os.system("make clean")
