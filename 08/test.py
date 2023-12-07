import unittest

import cjson


class TestStringMethods(unittest.TestCase):
    def test_loads(self):
        """
        Проверяем, что json.loads() возвращает  dict.
        """
        json_str = '{"abc": "456", "123f4": 12}'
        expected_dict = {"abc": "456", "123f4": 12}

        result = cjson.loads(json_str)

        self.assertEqual(result, expected_dict)

    def test_dumps(self):
        """
        Проверяем, что json.dumps() возвращает строку с JSON (исходным dict).
        """
        dict_obj = {"abc": "456", "123": "def", "123f4": 12}
        expected_json = '{"abc": "456", "123": "def", "123f4": 12}'

        result = cjson.dumps(dict_obj)

        self.assertEqual(result, expected_json)

    def test_roundtrip(self):
        """
        Проверяем, что строка JSON, преобразованная в dict и обратно в строку JSON, дает исходную строку JSON.
        А также делаем обратную проверку.
        """
        json_str = '{"abc": "456", "123": "def", "123f4": 12}'
        dict_obj = {"abc": "456", "123": "def", "123f4": 12}

        result_dumps_loads = cjson.loads(cjson.dumps(dict_obj))
        result_loads_dumps = cjson.dumps(cjson.loads(json_str))

        self.assertEqual(result_dumps_loads, dict_obj)
        self.assertEqual(result_loads_dumps, json_str)

    def test_invalid_object(self):
        """
        Проверяем, что при попытке подать другой объект вместо JSON-like, возникает ошибка.
        """
        fake_dict_str = "[1, 2, 3]"
        fake_dict = [1, 2, 3]

        with self.assertRaises(TypeError):
            cjson.loads(fake_dict_str)

        with self.assertRaises(TypeError):
            cjson.dumps(fake_dict)

    def test_invalid_type(self):
        """
        Проверяем, что возникает ошибка при использовании некорректных типов.
        """
        invalid_str_1 = '{"abc": "456", 123: "def", "123f4": 12}'
        invalid_str_2 = '{"abc": "456", "123": "def", "123f4": 1.2}'
        invalid_dict_1 = {"abc": "456", 123: "def", "123f4": 12}
        invalid_dict_2 = {"abc": "456", "123": "def", "123f4": [1, 2, 3]}

        with self.assertRaises(TypeError):
            cjson.loads(invalid_str_1)

        with self.assertRaises(TypeError):
            cjson.loads(invalid_str_2)

        with self.assertRaises(TypeError):
            cjson.dumps(invalid_dict_1)

        with self.assertRaises(TypeError):
            cjson.dumps(invalid_dict_2)


if __name__ == "__main__":
    unittest.main()
