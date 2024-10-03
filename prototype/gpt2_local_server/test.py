import torch.cuda
from transformers import GPT2LMHeadModel, GPT2Tokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling
from datasets import load_dataset  # Nowa funkcja do ładowania datasetów

print(torch.cuda.get_device_name(0))
print(torch.cuda.memory_summary(device=0))