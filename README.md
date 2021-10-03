# ARMAN: Pre-training with Semantically Selecting and Reordering of Sentences for Persian Abstractive Summarization

**Our paper has been accepted as a long paper in the EMNLP-2021 main conference and you can find the preprint version [here](https://arxiv.org/abs/2109.04098).**

Abstractive text summarization is one of the areas influenced by the emergence of pre-trained language models. Current pre-training works in abstractive summarization give more points to the summary with more words in common with the main text and pay less attention to the semantic similarity between generated sentences and the original document. We propose ARMAN, a Transformer-based encoder-decoder model pre-trained with three novel objectives to address this issue. In ARMAN, salient sentences from a document are selected according to a modified semantic score to be masked and form a pseudo summary. To summarize more accurately and similar to human writing patterns, we applied modified sentence reordering in the best setting. We evaluated our proposed models on six downstream Persian summarization tasks. Experimental results show that our proposed model achieves state-of-the-art performance on all six summarization tasks measured by ROUGE and BERTScore. Our models also outperform prior works in Textual Entailment, Question Paraphrasing, and Multiple Choice Question Answering. Finally, we established a human evaluation and show that using the semantic score significantly improves summarization results.

## Results

Our model ARMAN(MSR) got state-of-the-art results in 5 out of 6 Persian abstractive summarization datasets using ROUGE metric, and 6 out of 6 using BERTScore.

In the following table, the results are reported using `ROUGE-1/ROUGE-2/ROUGE-L` metrics.

| Dataset | ARMAN(MSR) | ARMAN(SS-100) | ARMAN(SH) | ARMAN(SS-80) | PEGASUS |
|---|---|---|---|---|---|
| PN-Summary | <sub>46.19/28.41/40.27<sub> | <sub>**46.33**/**28.57**/**40.38**<sub> | <sub>45.89/28.03/39.89<sub> | <sub>45.98/28.2/40.09<sub> | <sub>45.67/27.81/39.71<sub> |
| Wiki-Summary | <sub>**32.48**/**11.86**/**24.08**<sub> | <sub>32.36/11.78/24.1<sub> | <sub>32.04/11.78/23.83<sub> | <sub>32.27/11.72/23.91<sub> | <sub>31.98/11.63/23.79<sub> |
| VOA | <sub>**48.23**/**29.52**/**44.27**<sub> | <sub>47.73/28.95/43.89<sub> | <sub>46.96/27.88/42.93<sub> | <sub>47.91/28.9/43.75<sub> | <sub>47.55/28.68/43.57<sub> |
| Perkey(summary) | <sub>**63.59**/**52.87**/**60.3**<sub> | <sub>62.83/51.92/59.53<sub> | <sub>63.47/52.71/60.16<sub> | <sub>62.97/52.11/59.64<sub> | <sub>62.82/51.96/59.48<sub> |
| Perkey(title) | <sub>**54.81**/**40.17**/**52.51**<sub> | <sub>54.25/39.51/51.92<sub> | <sub>54.5/39.9/52.19<sub> | <sub>54.18/39.39/51.84<sub> | <sub>53.99/39.3/51.72<sub> |
| Tebyan | <sub>**37.79**/**21.85**/**31.98**<sub> | <sub>37.64/21.78/31.94<sub> | <sub>37.6/21.77/31.82<sub> | <sub>37.53/21.73/31.77<sub> | <sub>37.2/21.23/31.47<sub> |

In the following table, the results are reported using `Precision-BERTScore/Recall-BERTScore/F1-BERTScore` metrics.

| Dataset | ARMAN(MSR) | ARMAN(SH) | ARMAN(SS-80) | PEGASUS |
|---|---|---|---|---|
| PN-Summary | <sub>**80.14**/**79.84**/**79.93**<sub> | <sub>79.95/79.69/79.76<sub> | <sub>80.08/79.74/79.85<sub> | <sub>79.86/79.67/79.7<sub> |
| Wiki-Summary | <sub>**74.67**/**71.55**/**72.95**<sub> | <sub>74.25/71.43/72.68<sub> | <sub>74.24/71.48/72.71<sub> | <sub>74.29/71.31/72.64<sub> |
| VOA | <sub>**81.1**/**81.35**/**81.16**<sub> | <sub>80.64/80.91/80.71<sub> | <sub>81.02/81.13/81<sub> | <sub>80.84/81.13/80.92<sub> |
| Perkey(summary) | <sub>**86.54**/**86.24**/**86.33**<sub> | <sub>86.46/86.22/86.29<sub> | <sub>86.27/86.01/86.09<sub> | <sub>86.13/86.01/86.01<sub> |
| Perkey(title) | <sub>**83.93**/**83.59**/**83.71**<sub> | <sub>83.85/83.49/83.62<sub> | <sub>83.65/83.36/83.46<sub> | <sub>83.68/83.31/83.45<sub> |
| Tebyan | <sub>**75.49**/**75.46**/**75.4**<sub> | <sub>75.48/75.28/75.29<sub> | <sub>75.48/75.32/75.32<sub> | <sub>75.26/75.17/75.14<sub> |

Furthermore, we fine-tuned our models on the ParsiNLU dataset, and the results showed that ARMAN models could be used as Language model too! Our models get state-of-the-art results in 3 out of 4 tasks (virtually on natural part of the dataset). The results are reported in the following table. The results for other models are available in ParsiNLU paper (bold results are the results that were better than other reported models with at most 400M parameters, our models have around 220M).

| Task | Textual Entailment | Question Paraphrasing | Sentiment | Multiple-Choice Question Answering |
|---|---|---|---|---|
| Model | natural - translated | natural - translated  | food - movie | literature - common knowledge - math & logic |
| ARMAN(SS-80) | 54.5 - 50.6 | 82.5 - 74.8 | 51.4 - 47	| 37.7 - 25.7 - 47.7 |
| ARMAN(SS-100) | 54.2 - 53 | 79.9 - 72.8 | 50 - 52.9	| **41.4** - 27.4 - 43.1 |
| ARMAN(SH) | **55.5** - 52.9 | **82.6** - 75.1 | 56.7 - 42 | 34.6 - 28.6 - 45.4 |
| ARMAN(MSR) | 54.8 - 51.8 | 79.9 - 75.9 | 52 - 46 | 36.57 - 21.7 - **49.14** |
| PEGASUS | 54.5 - 52.6 | 80 - **76.1** | 51.9 - 56 | 40 - 27.7 - 45.1 |

Other important results about the ability of models for performing summarization in low resource scenarios are reported in our paper. Briefly, our model needs around 1K data points and 2K training steps to perform well on most summarization tasks.

## Link to models

This table contains pre-trained models that we trained.

| model | pre-trained | vocab |
|---|---|---|
| ARMAN(SS-80) | [download](https://drive.google.com/file/d/10KoubIxlvRst-DVYjL4PTRdZ-Uh5FQkz/view?usp=sharing) | [download](https://drive.google.com/file/d/1JiSqXYr7lhfoKSJrs7KCG52fxzooViAj/view?usp=sharing) |
| ARMAN(SS-100) | [download](https://drive.google.com/file/d/10RUMNL5rtfsnEIwIaPd8IkBF1lxpmxeh/view?usp=sharing) | [download](https://drive.google.com/file/d/19tNHVJ9RRmgtMsovpLCQSmRtEidhHkck/view?usp=sharing) |
| ARMAN(SH) | [download](https://drive.google.com/file/d/11pZOisSd_qtD_GH1nUqK4cI7PInp_lob/view?usp=sharing) | [download](https://drive.google.com/file/d/17kFNPJbMq-dsJ41US0l3oU75eqcl7lL2/view?usp=sharing) |
| ARMAN(MSR) | [download](https://drive.google.com/file/d/10zxKkWnUNi3mkmo9MrJY5n05aINu8I22/view?usp=sharing) | [download](https://drive.google.com/file/d/1I7jhDTOHCjyRxcrmA8v8QGL9a4-bZHJU/view?usp=sharing) |
| PEGASUS | [download](https://drive.google.com/file/d/1yHJeLIjFpq8Hn1l5Un7NSDFfktYVQ1GW/view?usp=sharing) | [download](https://drive.google.com/file/d/1_rcjl4dKxFVkhJK-nQ5S68dKqPW6nxYx/view?usp=sharing) |


This table contains fine-tuned models that we fine-tuned on summarization tasks.

| model | Perkey(summary) | Perkey(title) | Tebyan | Wiki Summary | VOA headlines | PN Summary | Vocab | 
|---|---|---|---|---|---|---|---|
| ARMAN(SS-80) | [download](https://drive.google.com/file/d/1048Dgax3XrL7TvltuAQmOcF16lyGpNLC/view?usp=sharing) | [download](https://drive.google.com/file/d/1054LQ30cYSPdWFCDuqzao1G2t4uLKKXP/view?usp=sharing) | [download](https://drive.google.com/file/d/10DKYTNu6uVu0RoXMQOakBiLtnL2pNQds/view?usp=sharing) | [download](https://drive.google.com/file/d/10IFJSMwDSiLY1B5uUaoLmGsXDCAggtL_/view?usp=sharing) | [download](https://drive.google.com/file/d/10FNEtaVVCklvp_-xUPvfbPjaNk24UwoM/view?usp=sharing) | [download](https://drive.google.com/file/d/1-pUijF1N7_LSk3jTFNVjuOV9Ab54vtN7/view?usp=sharing) | [download](https://drive.google.com/file/d/1JiSqXYr7lhfoKSJrs7KCG52fxzooViAj/view?usp=sharing) |
| ARMAN(SS-100) | [download](https://drive.google.com/file/d/10f-6OwVWS2mOPUXFpOEaWAChq3sbV3K7/view?usp=sharing) | [download](https://drive.google.com/file/d/10lVYoebLG5HSVrOSM5ZVows5zU5RIC9c/view?usp=sharing) | [download](https://drive.google.com/file/d/10ie80hhUmFxbGpCY3liTh0ILI8Xbjw-V/view?usp=sharing) | [download](https://drive.google.com/file/d/10z5F-xyknf6FoOEu44W5C4xAnNAfXQMb/view?usp=sharing) | [download](https://drive.google.com/file/d/10pHtaMPCtd6ySeLSwVYgvUDVlPZkVCvh/view?usp=sharing) | [download](https://drive.google.com/file/d/10TkvHyhMpzbQUGZGEqDa80J5JAH1uSd0/view?usp=sharing) | [download](https://drive.google.com/file/d/19tNHVJ9RRmgtMsovpLCQSmRtEidhHkck/view?usp=sharing) |
| ARMAN(SH) | [download](https://drive.google.com/file/d/12Hss4zW6hBBWgAqOApKnxGptXLkJylr_/view?usp=sharing) | [download](https://drive.google.com/file/d/12Rk-8KzGD1dh9bzX6p9g5a0SLXQPTM1I/view?usp=sharing) | [download](https://drive.google.com/file/d/12Sjrly-TZ4YC4CX8iTspi8hcVbKZFV6a/view?usp=sharing) | [download](https://drive.google.com/file/d/12kijEM1KY4EHa1W9M81JTSJtj0GGoXK_/view?usp=sharing) | [download](https://drive.google.com/file/d/12Va9GaI3C92iZxPsUpdYJBhi8ZtFzqm5/view?usp=sharing) | [download](https://drive.google.com/file/d/11teC7NB9mIFMxhkLAv0h5ds1ic9E_twD/view?usp=sharing) | [download](https://drive.google.com/file/d/17kFNPJbMq-dsJ41US0l3oU75eqcl7lL2/view?usp=sharing) |
| ARMAN(MSR) | [download](https://drive.google.com/file/d/11_yY2Cyohi7c8d1Q0dHjm7P0MPq6cByn/view?usp=sharing) | [download](https://drive.google.com/file/d/11bd5FuZHF5Zg0CjfxCLfXT9sXmBHtun7/view?usp=sharing) | [download](https://drive.google.com/file/d/11eZLQu3YRtroLBd6FFg3FHooZUohlLAk/view?usp=sharing) | [download](https://drive.google.com/file/d/11gJt8RJtvajoCCHBgZwewc8Mtm7eBL1k/view?usp=sharing) | [download](https://drive.google.com/file/d/11fdot0FNLyx9AXbe9P28jk3IMVdq1mTf/view?usp=sharing) | [download](https://drive.google.com/file/d/10zyAyS5_m6nR8Hc9m2dY8gwV-RAvxgHf/view?usp=sharing) | [download](https://drive.google.com/file/d/1I7jhDTOHCjyRxcrmA8v8QGL9a4-bZHJU/view?usp=sharing) |
| PEGASUS | [download](https://drive.google.com/file/d/1R_rZJ3RxvC-om4jlTaFcDa98ft_sG9st/view?usp=sharing) | [download](https://drive.google.com/file/d/1htkbo8DGne3zuNanSGyadDM5urZcXNwx/view?usp=sharing) | [download](https://drive.google.com/file/d/1Bxff1lAkGDhzg5mBQKtledXmg1NkHgVr/view?usp=sharing) | [download](https://drive.google.com/file/d/1TLxUVzASaehFAZe7fweDG1JnTmdB-M6d/view?usp=sharing) | [download](https://drive.google.com/file/d/1ubdZZcGN_Mchp9v5jin1SB-C29ow8KJ3/view?usp=sharing) | [download](https://drive.google.com/file/d/1k7KkOi8U4DTvZQkJTHQ81HE6ZgnPdSx8/view?usp=sharing) | [download](https://drive.google.com/file/d/1_rcjl4dKxFVkhJK-nQ5S68dKqPW6nxYx/view?usp=sharing) |
| TRANSFORMER | [download](https://drive.google.com/file/d/1a1PZ94XZEEfKmLbM3ET9PFMs0ROiqfyn/view?usp=sharing) | [download](https://drive.google.com/file/d/1TKV7msoJaZ-67SPW4lKnMYd9iIsQjxWH/view?usp=sharing) | [download](https://drive.google.com/file/d/1a70LpCFC5dHNSQq8fPd7LZBcDCVTWdBj/view?usp=sharing) | [download](https://drive.google.com/file/d/1GvVDYrkQr_6QIql-OnI1t9lZ6_GdAauj/view?usp=sharing) | [download](https://drive.google.com/file/d/14gJ0XRJ9kOtb7-Yfpor1JXw2SnfndjJQ/view?usp=sharing) | [download](https://drive.google.com/file/d/1rL5k3SujrRh68JCRpEbLSv0zFDLBLb30/view?usp=sharing) | [download](https://drive.google.com/file/d/1omKixvpIiyp_Vh4IeUYn-6VUcoiSjK6r/view?usp=sharing) |
| mT5 | [download](https://drive.google.com/file/d/1nz_yuBaoI-GxXST3xNaUX4TJIa6wG4If/view?usp=sharing) | [download](https://drive.google.com/file/d/1NKXJuCduMeCcAPVn-N_8l9fqjWUjisYA/view?usp=sharing) | [download](https://drive.google.com/file/d/1tPJQsmUGqq0Ogq7SJ-mbJRE6x1kensQ6/view?usp=sharing) | [download](https://drive.google.com/file/d/1Fu0XRslkPB7Zv1GagjRHbGp8cXHDikBZ/view?usp=sharing) | [download](https://drive.google.com/file/d/1MMpKhQGT0btIcLtudxLMOZgNrwzpR2g4/view?usp=sharing) | [download](https://drive.google.com/file/d/1G3Lekgh-Z8ac_2PHeizq7EHR5-80aBRR/view?usp=sharing) | [download](https://drive.google.com/file/d/1UgNo15RtIfO8GPrro9lEX5pr5O92-Jit/view?usp=sharing) |

This table contains fine-tuned models that we trained on NLU tasks.

| model | Entailment | Question Paraphrasing | Multiple Choice | Sentiment (Food) | Sentiment (Movie) | vocab |
|---|---|---|---|---|---|---|
| ARMAN(SS-80) | [download](https://drive.google.com/file/d/1-z5OOQVR-rhEgPDZ8czohYepJG9pJNBC/view?usp=sharing) | [download](https://drive.google.com/file/d/10-k57K_c2YF9lvo401LFrloQp7ZLizfV/view?usp=sharing) | [download](https://drive.google.com/file/d/1-uqKJTOxnex9HCiXNYi0Zh1_9GX3bBTB/view?usp=sharing) | [download](https://drive.google.com/file/d/101f9XAnQVM1sbmQGCaqMoQpAE4MD4sgH/view?usp=sharing) | [download](https://drive.google.com/file/d/100ymwmm86A0AbxtzL9hEL8C-zqiH6zoh/view?usp=sharing) | [download](https://drive.google.com/file/d/1JiSqXYr7lhfoKSJrs7KCG52fxzooViAj/view?usp=sharing) |
| ARMAN(SS-100) | [download](https://drive.google.com/file/d/10Vt2knCDgu1IadTjsmYQigtLEuTMK67U/view?usp=sharing) | [download](https://drive.google.com/file/d/10_de0PW0QjkbULKh7ND0Gm4XQraUczwa/view?usp=sharing) | [download](https://drive.google.com/file/d/10WlL8xwFdixhEy3UbyG8-0q4qDGFO178/view?usp=sharing) | [download](https://drive.google.com/file/d/10cMxEQY9iJnMuSi_Iiq34RpNDrvq07D2/view?usp=sharing) | [download](https://drive.google.com/file/d/10afjltA7ny7gNZNRpctj_iS7-5P735kY/view?usp=sharing) | [download](https://drive.google.com/file/d/19tNHVJ9RRmgtMsovpLCQSmRtEidhHkck/view?usp=sharing) |
| ARMAN(SH) | [download](https://drive.google.com/file/d/1262AmeC-ET65ls9x77_QbGkVad8GRTPG/view?usp=sharing) | [download](https://drive.google.com/file/d/127jNWuMwfjWOq_JqCXN2lIujWQFs_lci/view?usp=sharing) | [download](https://drive.google.com/file/d/11v1a_Jj31yHKxbJvtsZo1dKE95JGvj0Z/view?usp=sharing) | [download](https://drive.google.com/file/d/12GVulWyhKhDYcYyht7QnA27GR5UB9rHy/view?usp=sharing) | [download](https://drive.google.com/file/d/11vE2TQnbIzo4tcrb5OylnLgNAsPpqcnO/view?usp=sharing) | [download](https://drive.google.com/file/d/17kFNPJbMq-dsJ41US0l3oU75eqcl7lL2/view?usp=sharing) |
| ARMAN(MSR) | [download](https://drive.google.com/file/d/119jjbhPkr4Dcrzd3SDefJX1t6w7Gqjnq/view?usp=sharing) | [download](https://drive.google.com/file/d/11FNpesb4cM8XVc8ygG2MKiS4OTeEPJ7p/view?usp=sharing) | [download](https://drive.google.com/file/d/11AI6cxcGnxwqgZg_7kM1XX41fklAKi21/view?usp=sharing) | [download](https://drive.google.com/file/d/11XNGkyrSTiu_91n3GH_EIfCJw38jMCgC/view?usp=sharing) | [download](https://drive.google.com/file/d/11IuSk-0pNuYoCLfpT9Tkr9HzsTQyqyEJ/view?usp=sharing) | [download](https://drive.google.com/file/d/1I7jhDTOHCjyRxcrmA8v8QGL9a4-bZHJU/view?usp=sharing) |
| PEGASUS | [download](https://drive.google.com/file/d/13lBv8sEIRp89yTFuM_q-NIdValEnB0o3/view?usp=sharing) | [download](https://drive.google.com/file/d/1SDn6kRq7rPpYWmYCSye7Qk7l72k-Kel0/view?usp=sharing) | [download](https://drive.google.com/file/d/1DuNnBe3JeIldLQETtS3WGisPPqwfmh5o/view?usp=sharing) | [download](https://drive.google.com/file/d/15Asfds83lyAlUuEesMa2qI5vG0RUztbC/view?usp=sharing) | [download](https://drive.google.com/file/d/1UolVwuQubyP3FjYFXVY-rErR5S69vT9i/view?usp=sharing) | [download](https://drive.google.com/file/d/1_rcjl4dKxFVkhJK-nQ5S68dKqPW6nxYx/view?usp=sharing) |

## Link to Tebyan Dataset

The Tebyan cultural institute, which is affiliated to the organization "Sazman-e Tablighat-e Eslami", is one of the biggest and best known cultural institutes in Iran, and has cooperated with other cultural institutes in different fields for supporting cultural festivals and broadcasting their activities in the media. The activities of the institute's take place not only in Tehran, but also in the provincial centers, and 1,600,000 users visit its website each day. The Iranian deputy minister supported its activities for sport and youth on the website tebyan.net in Tehran.

We created the dataset by crawling the Tebyan website pages. Then we split it into `train/test/validation` sets. The dataset is publicly available for research purposes.

| train | validation | test |
|---|---|---|
| 78445 | 6922 | 6922 |
| [download](https://drive.google.com/file/d/1Rzon_Bl7jQ13IU0Hhda1Zfr5_lj2GDLA/view?usp=sharing) | [download](https://drive.google.com/file/d/1XBtDNJtInhfHQBhoe3-R8M3WzhUE4vNZ/view?usp=sharing) | [download](https://drive.google.com/file/d/1fd1doZNKjMPeKhZixyDfhZgVd47bdqq0/view?usp=sharing) |

## Pre-training or fine-tuning ARMAN?

The codes and guidelines on how to pre-train or fine-tune the model are available in the `pretraining` and `models` folder.

## huggingface models

We have converted our TF1 models into PyTorch models using the Huggingface library. You can find them [here](https://huggingface.co/alireza7). It should be noted that the reported results in our paper were produced using TF1 models, so we can not guarantee that you will get the same results using converted models.

## Citation

If you use this code, please consider citing our paper:


```
@misc{salemi2021arman,
      title={ARMAN: Pre-training with Semantically Selecting and Reordering of Sentences for Persian Abstractive Summarization}, 
      author={Alireza Salemi and Emad Kebriaei and Ghazal Neisi Minaei and Azadeh Shakery},
      year={2021},
      eprint={2109.04098},
      archivePrefix={arXiv},
      primaryClass={cs.CL}
}
```

