# Codes for Pre-training ARMAN

This section contains all of the codes that are needed for pretraining a new ARMAN model. These items should be done in the given order.

## Preprocessing pre-training datasets

For cleaning a corpus with our cleaning heuristic function, preprocessing pretraining datasets should be put in one text file with the name of `input.txt`, each line for a document. Next, run the `pretraining_preprocessing/clean_corpus.py` script, and it will make an `output.txt` file which is a cleaned version of the input file.

## Extracting Fasttext vectors 

First, the `bin` file for a `Fasttext` model should be download from [here](https://fasttext.cc/docs/en/crawl-vectors.html). Next, run `pretraining_vector_maker/vector_maker.py` script to generate desired files for pretraining dataset generation. This script will produce 4 outputs, `vectors.vec`, `words.txt`, `ngram.vec`, and `hash2index.txt`. These generated files are an essential part of the pre-training dataset generation algorithm.

## Vocab file

Toward generating `vocab_file`, it is enough to sample around 500K documents from your datasets and put them in a file with the name `input.txt`. Next, run `vocab_generator/vocab_generator.py` script, and `vocab_file` will be produced.

## Generating pretraining datasets

Our pre-training dataset generation has dependencies with [boost](https://www.boost.org/) and [OpenMP](https://www.openmp.org/) libraries, so before using it, these libraries should be installed. Next, generated files that extracted from fasttext vectors should be copied into `pretraining_data_generator/` directory. Then run the generation code:

```
cd pretraining_data_generator/
python main.py 
    --input_corpus=/*path to input corpus with a document per line*/ \
    --output_dir=/*path to output files*/ \
    --strategy=/*the strategy for generating datasets*/

```

Also, there are some other controlling parameters available. We provided help that could be seen by `--help` in `main.py' file.