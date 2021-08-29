import sentencepiece as spm

spm.SentencePieceTrainer.train(
    r"--input=input.txt --model_prefix=ARMAN_vocab --model_type=unigram --hard_vocab_limit=false --vocab_size=96000 --user_defined_symbols=<pad>,<mask>,<QA>,<SUMMARIZATION>,<KEYWORD>,<shuffle> --bos_id=-1 --eos_id=1 --unk_id=3"
)