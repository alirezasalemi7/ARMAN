from datasets import load_metric
from prettytable import PrettyTable

outs = []
seen = []
temp = False

with open("predictions.txt") as file:
  lines = file.readlines()
  for i,line in enumerate(lines):
    if temp:
      outs.append(line.replace("<n>","").replace("⁇ n>","").strip())
    if line[:6] == "-----:":
      seen.append(line[6:])
      temp = True
    else:
      temp = False 

targets = []
temp = False
with open("targets.txt") as file:
  lines = file.readlines()
  for i,line in enumerate(lines):
    if temp:
      targets.append(line.replace("<n>","").replace("⁇ n>","").strip())
    if line[:6] == "-----:":
      if line[6:] not in seen:
        print(line[6:])
      temp = True
    else:
      temp = False 

hyp = outs
ref = targets

rouge = load_metric('./rouge/')

scores = rouge.compute(
    predictions=hyp, 
    references=ref, 
    rouge_types=["rouge1", "rouge2", "rougeL", "rougeLsum","BERTScore"], 
    use_stemmer=False,
    lang='fa',
)

table = PrettyTable(title='Rouge Metric')
table.field_names = ["Score", "Type", "Precision", "Recall", "F-Measure"]
for i, score_name in enumerate(scores):
    score = dict(scores[score_name]._asdict())
    for score_type in score:
        values = score[score_type]
        table.add_row([f'{score_name}', f'{score_type}', f'{values.precision * 100:0.2f} ', f'{values.recall * 100:0.2f}', f'{values.fmeasure * 100:0.2f}'])

    if i < len(scores) - 1:
        table.add_row([''] * len(table.field_names))
        table.add_row(['***'] * len(table.field_names))
        table.add_row([''] * len(table.field_names))

rouge_table_valid = table
print(table.get_string())