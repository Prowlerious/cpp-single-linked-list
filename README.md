# Односвязный список (Single Linked List)

Проект в рамках обучения на Яндекс.Практикум.

## Краткое описание

Данная структура предоставляет возможность хранить данные одного типа и выполнять с ними следующие действия:
- записывать новый элемент в начало списка (PushFront);
- удалять элемент из начала списка (PopFront);
- вставлять новый элемент после указанной позиции (InsertAfter);
- удалять все элементы после указанной позиции (EraseAfter);
- очищать весь список (Clear);
- а также получать размер списка (GetSize) и проверять его на пустоту (IsEmpty).

Хранение данных осуществляется в динамической памяти, а связь между ними реализована посредством указателей (каждый элемент указывает на следующий, а последний на nullptr).

## Использование

Для использования данной структуры достаточно добавить `single-linked-list.h` в свой проект и включить его директивой #include. При объявлении объекта данного списка необходимо указать тип данных, которые будут в нем храниться:

```
SingleLinkedList<int> numbers{1, 1, 2, 3, 5, 8, 11};
```

Пример использования списка можно посмотреть в `main.cpp`.
Для сборки тестового проекта требуется любой компилятор, поддерживающий C++17: MinGW, G++, ...