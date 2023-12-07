import json
import random
from datetime import datetime, timedelta

import cjson
import ujson
from faker import Faker


def generate_data(size):
    faker = Faker()
    big_json = {}
    for _ in range(size):
        rand = random.randint(1, 2)
        if rand == 1:
            big_json[faker.ean(length=8)] = random.randint(1, 100)
            # big_json[faker.first_name()] = faker.cryptocurrency_name()
        else:
            big_json[faker.ean(length=8)] = random.randint(1, 100)
    return big_json


def stress_test(json_num=100, json_size=1_000):
    ujson_time = timedelta(0)
    json_time = timedelta(0)
    cjson_time = timedelta(0)
    for _ in range(json_num):
        big_json = generate_data(json_size)

        start = datetime.now()
        ujson_str = ujson.dumps(big_json)
        ujson_dict = ujson.loads(ujson_str)
        ujson_time += datetime.now() - start

        start = datetime.now()
        json_str = json.dumps(big_json)
        json_dict = json.loads(json_str)
        json_time += datetime.now() - start

        start = datetime.now()
        cjson_str = cjson.dumps(big_json)
        cjson_dict = cjson.loads(cjson_str)
        cjson_time += datetime.now() - start

        assert ujson_dict == json_dict == cjson_dict

    print("Elapsed time:")
    print(f"\tujson: {ujson_time.total_seconds() * 1000:.3f}")
    print(f"\tjson: {json_time.total_seconds() * 1000:.3f}")
    print(f"\tcjson: {cjson_time.total_seconds() * 1000:.3f}")


if __name__ == "__main__":
    stress_test()
    # ожидается примерно такой результат
    # Elapsed time:
    #     ujson: 16.153
    #     json: 29.222
    #     cjson: 209.569

    # ujson ожидаемо самый быстрый
    # обычный json в ~2 раза медленнее
    # наша реализация медленнее в 10 раз в сравнении с обычным json
