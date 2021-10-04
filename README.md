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
| ARMAN(SS-80) | [download](https://drive.google.com/file/d/14zNKNGTyUzkGG8o3Ub95B6VAs9PixsHH/view?usp=sharing) | [download](https://drive.google.com/file/d/1gaTn_vWUi1pus-Zx4T6ZRZv7MDJfl7Hn/view?usp=sharing) |
| ARMAN(SS-100) | [download](https://drive.google.com/file/d/1d-pUJ5BInKQdUfXEuBfc9hr1a1Yw3Fxm/view?usp=sharing) | [download](https://drive.google.com/file/d/1U1L_IangBi2mrahN-qmfov6cSLSkSSrA/view?usp=sharing) |
| ARMAN(SH) | [download](https://drive.google.com/file/d/1f6sEvFpn80eXYwJmFWc5dikzmYECY_vk/view?usp=sharing) | [download](https://drive.google.com/file/d/1x8HPakkCNuecWl-BypKb_zL1Ag_hKvAX/view?usp=sharing) |
| ARMAN(MSR) | [download](https://drive.google.com/file/d/1YT2AxSw5euWzpVEFsXbGXYrqOOC9zVz5/view?usp=sharing) | [download](https://drive.google.com/file/d/1Gk65FMF9qryzpwco2LFe0X5W9V789qzQ/view?usp=sharing) |
| PEGASUS | [download](https://drive.google.com/file/d/1yHJeLIjFpq8Hn1l5Un7NSDFfktYVQ1GW/view?usp=sharing) | [download](https://drive.google.com/file/d/1_rcjl4dKxFVkhJK-nQ5S68dKqPW6nxYx/view?usp=sharing) |


This table contains fine-tuned models that we fine-tuned on summarization tasks.

| model | Perkey(summary) | Perkey(title) | Tebyan | Wiki Summary | VOA headlines | PN Summary | Vocab | 
|---|---|---|---|---|---|---|---|
| ARMAN(SS-80) | [download](https://drive.google.com/file/d/1EZGvdhwBdsXmY87TPgANR2rUx0hvNw8O/view?usp=sharing) | [download](https://drive.google.com/file/d/1K_CVciDji5RFTOhZ38P0HbIkFDE3YPjn/view?usp=sharing) | [download](https://drive.google.com/file/d/13o2FwGAbuQ0synhTpprHJm4A929ir2ux/view?usp=sharing) | [download](https://drive.google.com/file/d/1dUXvUt1MwOWJcawHviJRaWAbhn9JbSEu/view?usp=sharing) | [download](https://drive.google.com/file/d/1vPCmg6u_JM7t0XXBLnnAkNV9ZkCTB2El/view?usp=sharing) | [download](https://drive.google.com/file/d/1NvYJCB6N85lasKoPMVa1992SVqN9CWO7/view?usp=sharing) | [download](https://drive.google.com/file/d/1gaTn_vWUi1pus-Zx4T6ZRZv7MDJfl7Hn/view?usp=sharing) |
| ARMAN(SS-100) | [download](https://drive.google.com/file/d/1L7HXOKF_2OphTXuur9tTpuzogR4kmG8Y/view?usp=sharing) | [download](https://drive.google.com/file/d/1CnuB8LGw_1SmmTUBeLMz723PWPJxUYuc/view?usp=sharing) | [download](https://drive.google.com/file/d/1x7jpjdOFcdfzpb9ZqNAbSELhouXEtCyz/view?usp=sharing) | [download](https://drive.google.com/file/d/1ReMQw9xRT1Oh6p2BTQfxh8sx8GMWk47X/view?usp=sharing) | [download](https://drive.google.com/file/d/1CC0Z3NPU_pLN_BJkmlvasmoM4XOUUR6_/view?usp=sharing) | [download](https://drive.google.com/file/d/1SNy-jEpUcQJTqKLSAHY-0ycTiixfrgS9/view?usp=sharing) | [download](https://drive.google.com/file/d/1U1L_IangBi2mrahN-qmfov6cSLSkSSrA/view?usp=sharing) |
| ARMAN(SH) | [download](https://drive.google.com/file/d/1H4maWRFCDMxbl2005M4XH_byG1nMVg68/view?usp=sharing) | [download](https://drive.google.com/file/d/1MyFBlT4-PyixsZV1qLAv8vg5RqbVmPME/view?usp=sharing) | [download](https://drive.google.com/file/d/1Lewj_nWvjgLEsvqvqBOnLzL-cXsrvHjG/view?usp=sharing) | [download](https://drive.google.com/file/d/17bDe9D2XPATD0KaCm8UcJXaRWD_oE3Bd/view?usp=sharing) | [download](https://drive.google.com/file/d/1LWISK8lqJNgfBm8-c5H0Q7DbuA2LMI2j/view?usp=sharing) | [download](https://drive.google.com/file/d/1qgyaM4tOH9Ylnf1piAsh4_uSwrdUtVP3/view?usp=sharing) | [download](https://drive.google.com/file/d/1x8HPakkCNuecWl-BypKb_zL1Ag_hKvAX/view?usp=sharing) |
| ARMAN(MSR) | [download](https://drive.google.com/file/d/1KwNo9y_4gDPq1C6Yk5xClteTqenNAxi2/view?usp=sharing) | [download](https://drive.google.com/file/d/1ljT8p5YVgsAj2hBdAWp8Fv3lZYpJ_ii5/view?usp=sharing) | [download](https://drive.google.com/file/d/11RA53rDA6NyG1B1xhDaU2B9Uw9iJmK9P/view?usp=sharing) | [download](https://drive.google.com/file/d/1JZcFiWKvdOVZoJS_aIi2cgremXw71UHG/view?usp=sharing) | [download](https://drive.google.com/file/d/1Puyx09E49gYJ537K-j6ZpwrzREZwzpU1/view?usp=sharing) | [download](https://drive.google.com/file/d/1xx_bx1LJcTt6T9WutGyzNyIdG5NbUttB/view?usp=sharing) | [download](https://drive.google.com/file/d/1Gk65FMF9qryzpwco2LFe0X5W9V789qzQ/view?usp=sharing) |
| PEGASUS | [download](https://drive.google.com/file/d/1R_rZJ3RxvC-om4jlTaFcDa98ft_sG9st/view?usp=sharing) | [download](https://drive.google.com/file/d/1htkbo8DGne3zuNanSGyadDM5urZcXNwx/view?usp=sharing) | [download](https://drive.google.com/file/d/1Bxff1lAkGDhzg5mBQKtledXmg1NkHgVr/view?usp=sharing) | [download](https://drive.google.com/file/d/1TLxUVzASaehFAZe7fweDG1JnTmdB-M6d/view?usp=sharing) | [download](https://drive.google.com/file/d/1ubdZZcGN_Mchp9v5jin1SB-C29ow8KJ3/view?usp=sharing) | [download](https://drive.google.com/file/d/1k7KkOi8U4DTvZQkJTHQ81HE6ZgnPdSx8/view?usp=sharing) | [download](https://drive.google.com/file/d/1_rcjl4dKxFVkhJK-nQ5S68dKqPW6nxYx/view?usp=sharing) |
| TRANSFORMER | [download](https://drive.google.com/file/d/1a1PZ94XZEEfKmLbM3ET9PFMs0ROiqfyn/view?usp=sharing) | [download](https://drive.google.com/file/d/1TKV7msoJaZ-67SPW4lKnMYd9iIsQjxWH/view?usp=sharing) | [download](https://drive.google.com/file/d/1a70LpCFC5dHNSQq8fPd7LZBcDCVTWdBj/view?usp=sharing) | [download](https://drive.google.com/file/d/1GvVDYrkQr_6QIql-OnI1t9lZ6_GdAauj/view?usp=sharing) | [download](https://drive.google.com/file/d/14gJ0XRJ9kOtb7-Yfpor1JXw2SnfndjJQ/view?usp=sharing) | [download](https://drive.google.com/file/d/1rL5k3SujrRh68JCRpEbLSv0zFDLBLb30/view?usp=sharing) | [download](https://drive.google.com/file/d/1omKixvpIiyp_Vh4IeUYn-6VUcoiSjK6r/view?usp=sharing) |
| mT5 | [download](https://drive.google.com/file/d/1nz_yuBaoI-GxXST3xNaUX4TJIa6wG4If/view?usp=sharing) | [download](https://drive.google.com/file/d/1NKXJuCduMeCcAPVn-N_8l9fqjWUjisYA/view?usp=sharing) | [download](https://drive.google.com/file/d/1tPJQsmUGqq0Ogq7SJ-mbJRE6x1kensQ6/view?usp=sharing) | [download](https://drive.google.com/file/d/1Fu0XRslkPB7Zv1GagjRHbGp8cXHDikBZ/view?usp=sharing) | [download](https://drive.google.com/file/d/1MMpKhQGT0btIcLtudxLMOZgNrwzpR2g4/view?usp=sharing) | [download](https://drive.google.com/file/d/1G3Lekgh-Z8ac_2PHeizq7EHR5-80aBRR/view?usp=sharing) | [download](https://drive.google.com/file/d/1UgNo15RtIfO8GPrro9lEX5pr5O92-Jit/view?usp=sharing) |

This table contains fine-tuned models that we trained on NLU tasks.

| model | Entailment | Question Paraphrasing | Multiple Choice | Sentiment (Food) | Sentiment (Movie) | vocab |
|---|---|---|---|---|---|---|
| ARMAN(SS-80) | [download](https://drive.google.com/file/d/1OdrGZpsJztA0zKVXDrKpViwOgR1MIxHB/view?usp=sharing) | [download](https://drive.google.com/file/d/148FVp_PeExHBa-I0BeXfiO-crSFn5Zyq/view?usp=sharing) | [download](https://drive.google.com/file/d/1AeosoSLuM3_wwen9veaW4aIMynMa5a1t/view?usp=sharing) | [download](https://drive.google.com/file/d/1KlGNql68nzrN35vrm95GuwPyJGyWIfpr/view?usp=sharing) | [download](https://drive.google.com/file/d/1Gh8aRZUAm0hLxHhskWBYh4kEvj8P0-zo/view?usp=sharing) | [download](https://drive.google.com/file/d/1gaTn_vWUi1pus-Zx4T6ZRZv7MDJfl7Hn/view?usp=sharing) |
| ARMAN(SS-100) | [download](https://drive.google.com/file/d/1aV0YvfyUdVmgibGjEj270KxChy08GvNE/view?usp=sharing) | [download](https://drive.google.com/file/d/1OQfG2tvsUO0cMsSFROAi7QT5DUBwUGHj/view?usp=sharing) | [download](https://drive.google.com/file/d/1fPitntI3I_OlYVjK8uvpG2XMb7IARUDo/view?usp=sharing) | [download](https://drive.google.com/file/d/19Nw7JFdXDQ-SFA-y9NyI98GIvqMB6LUY/view?usp=sharing) | [download](https://drive.google.com/file/d/1VbMt1Qdq77PI7ijCe5ZZsW0sq3RLLIiX/view?usp=sharing) | [download](https://drive.google.com/file/d/1U1L_IangBi2mrahN-qmfov6cSLSkSSrA/view?usp=sharing) |
| ARMAN(SH) | [download](https://drive.google.com/file/d/1AyJyOKCwjCfXzwr5Mw0nlvV4Vn5XeLfv/view?usp=sharing) | [download](https://drive.google.com/file/d/1pPDQcFHbl1Nq8kuTeykWtTJvjVSBvvWr/view?usp=sharing) | [download](https://drive.google.com/file/d/1_4jEAMLwOxU7r1iafsV0dfNEdbECkYwU/view?usp=sharing) | [download](https://drive.google.com/file/d/1gD-wExk8LTHVAx0yM-ZeBX8yGEGWfJfL/view?usp=sharing) | [download](https://drive.google.com/file/d/13LuMp8w1034j-lLQbgyCfRGhPwAKdiPE/view?usp=sharing) | [download](https://drive.google.com/file/d/1x8HPakkCNuecWl-BypKb_zL1Ag_hKvAX/view?usp=sharing) |
| ARMAN(MSR) | [download](https://drive.google.com/file/d/168L8ahaPk32H0ObwoUlNI6dNyHI1FU8A/view?usp=sharing) | [download](https://drive.google.com/file/d/1HJpLrID94oMf1LeuLBOSzeQ0m97LzEeE/view?usp=sharing) | [download](https://drive.google.com/file/d/1bD1YC1z4mBy2Pc7teyLMnvbUkFP7exrE/view?usp=sharing) | [download](https://drive.google.com/file/d/1SV6Hbo3OnP503WzEow0bHyU-gnXKLcYn/view?usp=sharing) | [download](https://drive.google.com/file/d/1SpJm6ZOJm9uD5plwE_AyijOsO2PAm1Rt/view?usp=sharing) | [download](https://drive.google.com/file/d/1Gk65FMF9qryzpwco2LFe0X5W9V789qzQ/view?usp=sharing) |
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

