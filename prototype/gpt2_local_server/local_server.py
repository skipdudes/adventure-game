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
model = GPT2LMHeadModel.from_pretrained(model_path).to("cuda")

# Model danych do komunikacji
class RequestData(BaseModel):
    prompt: str

# Funkcja generująca tekst
def generate_text(prompt):
    inputs = tokenizer(prompt, return_tensors="pt", padding=True, truncation=True).to("cuda")
    input_ids = inputs['input_ids']
    attention_mask = inputs['attention_mask']
    outputs = model.generate(
        input_ids,
        attention_mask=attention_mask,
        max_length=100,
        repetition_penalty=3.0,     # kara za zapetlanie tektu
        num_return_sequences=2,     # ilosc zwroconych alternatywnych odpowiedzi
        num_beams=3,                # ilosc promieni, jesli >1 to wylacza greedy decoding, czyli wybiera rozne tokeny, a nie tylko te najbardziej prawdopodobne.
        do_sample=True,             # wlacz losowa probke
        early_stopping=True,        # zatrzymuje generowanie np po zakonczeniu zdania
        #temperature=1.0,            # <1.0 zwraca odpowiedzi bezpieczne, >1.0 generuje bardziej kreatywnie, ale mniej spojnie, np. 0.7-1.5
        #top_k=50,                   # model wybiera z 50 najbardziej prawdopodobnych tokenow, alternatywa top_p=0.9 czyli wybiera sposrod tokenow o prawdopodobienstwie 90%
        length_penalty=1.0)         # jesli <1.0 to preferuje krotsze odpowiedzi, >1.0 preferuje dluzsze

    return tokenizer.decode(outputs[0], skip_special_tokens=True)


# Endpoint do przetwarzania zapytań POST
@app.post("/generate/")
async def generate_text_endpoint(data: RequestData):
    response = generate_text(data.prompt)
    print(data.prompt)
    return response

# Uruchomienie serwera
if __name__ == "__main__":
#    uvicorn.run(app, host="0.0.0.0", port=8000)
    print(generate_text("You are a medieval city guard standing by the gate. A Traveller approaches and asks: Where is the town hall ?\n"))

