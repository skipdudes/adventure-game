from transformers import GPT2LMHeadModel, GPT2Tokenizer, Trainer, TrainingArguments, DataCollatorForLanguageModeling
from datasets import load_dataset  # Nowa funkcja do ładowania datasetów

# Wczytanie tokenizer'a i modelu GPT-2
tokenizer = GPT2Tokenizer.from_pretrained("gpt2-large")
model = GPT2LMHeadModel.from_pretrained("gpt2-large")

tokenizer.pad_token = tokenizer.eos_token

# Funkcja tworząca dataset z pliku .txt przy użyciu Hugging Face datasets
def load_custom_dataset(file_path, tokenizer, block_size=128):
    # Wczytanie danych z pliku .txt przy użyciu funkcji load_dataset
    dataset = load_dataset('text', data_files={'train': file_path})

    # Tokenizacja wczytanych danych
    def tokenize_function(examples):
        return tokenizer(examples['text'], truncation=True, padding='max_length', max_length=block_size)

    # Tokenizowanie datasetu
    tokenized_datasets = dataset.map(tokenize_function, batched=True)
    return tokenized_datasets['train']


# Sprawdzenie pliku z wytycznymi (np. "wytyczne.txt")
try:
    dataset = load_custom_dataset("wytyczne.txt", tokenizer)
    if len(dataset) == 0:
        raise ValueError("Dataset is empty!")
except Exception as e:
    print(f"Error loading dataset: {e}")
    raise

# Ustawienia treningu
training_args = TrainingArguments(
    output_dir="./results",
    overwrite_output_dir=True,
    num_train_epochs=1,
    per_device_train_batch_size=2,
    save_steps=10_000,
    save_total_limit=2,
    prediction_loss_only=True,
)

# Użycie DataCollator
data_collator = DataCollatorForLanguageModeling(
    tokenizer=tokenizer, mlm=False,
)

# Trening modelu
trainer = Trainer(
    model=model,
    args=training_args,
    data_collator=data_collator,
    train_dataset=dataset,
)

trainer.train()

# Zapisanie modelu po fine-tuningu
model.save_pretrained("fine_tuned_gpt2")
tokenizer.save_pretrained("fine_tuned_gpt2")
