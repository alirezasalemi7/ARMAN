import re
import sys
import langdetect

max_mem_size = 4
sentence_sep_pattern = "([!\.\?⸮؟]+)[ \n]+"
min_line_per_doc = 3
min_word_per_line = 5
end_marks = ("!",".","?","⸮","؟")
words_should_be_filtered_out = [
    "برچسب:",
    "برچسب ها :",
    "» مطالب و مقالات »",
    "برچسبها:",
    "برچسب :",
    "برچسبها :",
    "برچسب ها:",
    "http://",
    "حقوق این وب سایت محفوظ",
    "فراموشی رمز ورود",
    "https://",
    "ورود به سایت",
    "... ادامه خبر",
    "بیشتر بدانید ...",
    "...",
    "کلید واژه ها:",
    "کلید واژه ها :",
    "کلید واژهها:",
    "archive :",
    "archives :",
    "archive:",
    "archives:",
    "Archive :",
    "Archives :",
    "این مطلب را به اشتراک بگذارید",
    "کلیه حقوق مادی و معنوی",
    "هیچ نظری هنوز ثبت نشده است",
    "گر قبلا در بیان ثبت نام کرده اید می توانید ابتدا وارد شوید",
    "پست الکترونيک شما میتوانید از این تگهای html استفاده کنید",
    "پست الکترونیک برای عموم قابل مشاهده باشد اخطار!",
    "Archive:",
    "Archives:",
    "(نظر، انتقاد، پیشنهاد ...)",
    "...",
    ". . ."
]

duplicate_set = set()

def clean_text(text):
    pattern = re.compile(sentence_sep_pattern)
    text = pattern.sub(r'\1\n', text)
    sentences = [sentence.replace('\n', ' ').strip() for sentence in text.split('\n') if sentence.strip()]
    final_sentences = []
    for sentence in sentences:
        sentence_hash = hash(sentence)
        if sentence_hash in duplicate_set:
            continue
        else:
            duplicate_set.add(sentence_hash)
        if len(sentence.split()) < min_word_per_line:
            continue
        if not sentence.endswith(end_marks):
            continue
        if "javascript" in sentence:
            continue
        if "lorem ipsum" in sentence:
            continue
        if "{" in sentence:
            continue
        remove = False
        for filter_word in words_should_be_filtered_out:
            if filter_word in sentence:
                remove = True
                break
        if remove:
            continue
        final_sentences.append(sentence)
    if len(final_sentences) < min_line_per_doc:
        return None
    cleaned = " ".join(final_sentences).replace('\n'," ")
    if langdetect.detect(cleaned) != "fa":
        return None
    return cleaned

def readSingleText(file):
    return_text = ""
    while True:
        line = file.readline().strip()
        if not line:
            return return_text
        return_text += line + "\n"


with open("input.txt",encoding="utf8") as inFile, open("output.txt","w",encoding="utf8") as outFile:
    i = 0
    total = 0
    while True:
        i += 1
        line = readSingleText(inFile)
        if not line:
            break
        line = clean_text(line)
        if line:
            total += 1
            outFile.write(line+"\n")
        if i % 10000 == 0 and sys.getsizeof(duplicate_set) > max_mem_size * 1e9:
            duplicate_set = set()
    print(total)
