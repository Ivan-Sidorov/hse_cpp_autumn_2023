#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

static PyObject* dumps(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* dict;
    PyObject* key;
    PyObject* value;
    // получаем объект
    if (!PyArg_ParseTuple(args, "O", &dict)) {
        PyErr_SetString(PyExc_TypeError,
                        "Failed to parse object. It must be a dict!");
        return NULL;
    }
    // проверяем, что это словарь
    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError,
                        "Failed to parse object. It must be a dict!");
        return NULL;
    }

    Py_ssize_t pos = 0;
    size_t cnt = 0;
    PyObject* dump_string = PyUnicode_FromString("{");
    // проходимся по словарю
    while (PyDict_Next(dict, &pos, &key, &value)) {
        PyObject* curr_key = NULL;
        // проверяем, что ключ имеет тип строка
        if (PyUnicode_Check(key)) {
            curr_key = key;
        } else {
            PyErr_SetString(PyExc_TypeError,
                            "Failed to parse object. Key must be a string!");
            return NULL;
        }

        PyObject* curr_value = NULL;
        if (PyUnicode_Check(value)) {
            // если значение имеет тип строка, то формируем "key": "value"
            curr_value =
                PyUnicode_FromFormat("\"%S\": \"%S\"", curr_key, value);
        } else if (PyLong_Check(value)) {
            // если число, то переводим его в строку и формируем "key": value
            int curr_value_int = PyLong_AsLong(value);
            char* curr_value_str =
                malloc(sizeof(char) * (int)log10(curr_value_int));
            sprintf(curr_value_str, "%d", curr_value_int);
            curr_value = PyUnicode_FromFormat(
                "\"%S\": %S", curr_key, Py_BuildValue("s", curr_value_str));
            free(curr_value_str);
        } else {
            // иначе ошибка, так как поддерживаем только два типа
            PyErr_SetString(PyExc_TypeError,
                            "Failed to parse object. Value must be a string or "
                            "an integer!");
            return NULL;
        }

        if (cnt == 0) {
            // если начало, то подставляем открывающую скобку
            dump_string = PyUnicode_Concat(dump_string, curr_value);
        } else {
            // иначе просто добавляем запятую
            dump_string =
                PyUnicode_Concat(dump_string, PyUnicode_FromString(", "));
            dump_string = PyUnicode_Concat(dump_string, curr_value);
        }
        cnt++;
    }
    // в конце добавляем закрывающую скобку
    dump_string = PyUnicode_Concat(dump_string, PyUnicode_FromString("}"));

    return dump_string;
}

void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

static PyObject* loads(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* dict;
    PyObject* key;
    PyObject* value;

    if (!(dict = PyDict_New())) {
        PyErr_SetString(PyExc_Exception, "Failed to create dict!");
        return NULL;
    }

    char* dict_str_in;
    // получаем строку
    if (!PyArg_ParseTuple(args, "s", &dict_str_in)) {
        PyErr_SetString(PyExc_TypeError,
                        "Failed to parse object. It must be a string!");
        return NULL;
    }
    // создаем копию полученной строки, так как будем ее изменять
    char* dict_str = malloc(sizeof(char) * (strlen(dict_str_in) + 1));
    strcpy(dict_str, dict_str_in);

    char* objects;
    char* dict_key;
    char dict_value[100];
    static char const* delimeters = "{}:, ";
    objects = strtok(dict_str, delimeters);
    while (objects != NULL) {
        dict_key = objects;
        // разбиваем на токены, будем обрабатывать их по очереди
        objects = strtok(NULL, delimeters);
        if (strchr(objects, '[') || strchr(objects, ']')) {
            PyErr_SetString(PyExc_TypeError,
                            "Failed to create dict due to invalid type!");
            free(dict_str);
            return NULL;
        }
        strcpy(dict_value, objects);

        // проверяем корректность кавычек
        if (!((dict_key[0] == '\"') &&
              (dict_key[strlen(dict_key) - 1] == '\"'))) {
            PyErr_SetString(
                PyExc_TypeError,
                "Failed to create string for key due to invalid braces!");
            free(dict_str);
            return NULL;
        }

        // удаляем лишние кавычки у строки "string" -> string
        // и записываем ключ в key
        remove_all_chars(dict_key, '\"');
        if (!(key = Py_BuildValue("s", dict_key))) {
            PyErr_SetString(PyExc_Exception,
                            "Failed to create string for key!");
            free(dict_str);
            return NULL;
        }

        char* end_check;
        strtol(dict_value, &end_check, 10);
        if (*end_check == '\0') {
            // переменная число
            int val = atoi(dict_value);
            if (!(value = Py_BuildValue("i", val))) {
                PyErr_SetString(PyExc_Exception,
                                "Failed to create integer for value!");
                free(dict_str);
                return NULL;
            }
        } else {
            // переменная строка
            if (!Py_BuildValue("s", dict_value)) {
                PyErr_SetString(PyExc_Exception,
                                "Failed to create string for value!");
                free(dict_str);
                return NULL;
            }

            // проверка кавычек, как и раньше
            if (!((dict_value[0] == '\"') &&
                  (dict_value[strlen(dict_value) - 1] == '\"'))) {
                PyErr_SetString(
                    PyExc_TypeError,
                    "Failed to create string for value due to invalid braces!");
                free(dict_str);
                return NULL;
            }

            // удаление кавычек, как и раньше
            remove_all_chars(dict_value, '\"');
            value = Py_BuildValue("s", dict_value);
        }
        if (PyDict_SetItem(dict, key, value) < 0) {
            PyErr_SetString(PyExc_Exception,
                            "Failed to add (key, value) pair to the dict");
            free(dict_str);
            return NULL;
        }
        objects = strtok(NULL, delimeters);
    }
    free(dict_str);
    return dict;
}

static PyMethodDef methods_def[] = {
    {"loads", loads, METH_VARARGS,
     "Deserialize JSON formatted string to a dict."},
    {"dumps", dumps, METH_VARARGS,
     "Serialize dict to a JSON formatted string."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "cjson", NULL,
                                        -1, methods_def};

PyMODINIT_FUNC PyInit_cjson(void) { return PyModule_Create(&module_def); }