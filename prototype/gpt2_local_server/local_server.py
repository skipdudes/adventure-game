from fastapi import FastAPI
from pydantic import BaseModel
from transformers import GPT2LMHeadModel, GPT2Tokenizer
import uvicorn
import torch

print(torch.cuda.is_available())

app = FastAPI()

# Wczytanie modelu i tokenizera GPT-2
model_path = "fine_tuned_gpt2"
tokenizer = GPT2Tokenizer.from_pretrained(model_path)
model = GPT2LMHeadModel.from_pretrained(model_path)

# Model danych do komunikacji
class RequestData(BaseModel):
    prompt: str

# Funkcja generująca tekst
def generate_text(prompt, max_length=300):
    inputs = tokenizer(prompt, return_tensors="pt", padding=True, truncation=True)
    input_ids = inputs['input_ids']
    attention_mask = inputs['attention_mask']
    outputs = model.generate(input_ids, attention_mask=attention_mask, max_length=max_length, repetition_penalty=2.0)
    return tokenizer.decode(outputs[0], skip_special_tokens=True)


# Endpoint do przetwarzania zapytań POST
@app.post("/generate/")
async def generate_text_endpoint(data: RequestData):
    response = generate_text(data.prompt)
    print(data.prompt)
    return {"response": response}

# Uruchomienie serwera
if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)

