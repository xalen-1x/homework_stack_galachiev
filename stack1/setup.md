# Stack: локальная сборка и тесты

Реализация стека строк - `stack.hpp` (условия задачи - в [readme.md](readme.md)).
Все тесты запускаются **локально** на Linux, macOS или WSL.
Нативный Windows не поддерживается - используй WSL.

## Что нужно установить

Три вещи (отдельно GoogleTest ставить **не надо** - CMake скачает его сам,
git тоже не нужен):

- CMake **≥ 3.16**
- C++-компилятор с поддержкой **C++20** (gcc 10+ или clang 12+);
  если компилятор умеет C++23 - сборка автоматически пойдёт на C++23
- `make`

Интернет нужен **только при первом configure** - CMake скачает GoogleTest
с github. После этого кэш лежит в папке сборки и сеть больше не нужна.

## Установка (один раз)

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential cmake
```

### macOS

1. Компилятор и `make` - Command Line Tools:

   ```bash
   xcode-select --install
   ```

2. CMake (в Command Line Tools его **нет**):

   ```bash
   brew install cmake
   ```

   Нет Homebrew - скачай `.dmg` с [cmake.org](https://cmake.org/download/)
   и распакуй (sudo не нужен).

Проверка:

```bash
cmake --version   # должно быть >= 3.16
```

## Сборка и запуск тестов

Скрипт собирает проект и прогоняет все тесты:

```bash
bash build.sh
```

В конце печатается явный итог - `✅ Все тесты прошли` или
`❌ Тесты не прошли` (имя упавшего теста будет в выводе ctest).

Один конкретный тест (сборка + только этот кейс):

```bash
bash build.sh StackTest.PopLIFO
```

Вручную - те же шаги, что делает `build.sh`:

```bash
cmake -B build
cmake --build build
cd build && ctest --output-on-failure
```

**Ожидаемый результат:** все тесты зелёные -
37 тестов (`Constness.Test`, `StackTest.PopLIFO`, …), в конце
`100% tests passed, 0 tests failed out of 37`.

Всё решение пишется в **`stack.hpp`** (header-only) - тесты и play
подключают его напрямую. Других `.cpp` в проекте нет - не создавай их.
Проект собирается на **C++23**, если компилятор его поддерживает,
иначе на **C++20**.

### Play (своя песочница)

```bash
./build/play_stack
```

Печатает:

```
world
hello
```

В `play/main.cpp` можно писать свой сценарий использования стека -
таргет всегда включается в сборку, удобно для отладки.

### Тесты по отдельности

```bash
bash build.sh StackTest.PopLIFO                       # собрать + один тест через ctest
./build/test_stack --gtest_filter=StackTest.PopLIFO   # то же напрямую
./build/test_stack                                    # весь набор напрямую
cd build && ctest -N                                  # список всех тестов
```

### Как искать ошибки в решении

Каждый gtest-кейс зарегистрирован в ctest отдельно, поэтому упавший тест
называется прямо в выводе скрипта:

```
2/37 Test #2: StackTest.ExtraPop ...***Failed
```

или при зависании:

```
1/37 Test #1: Constness.Test ...***Timeout 7.00 sec
```

- **`***Failed`** - не прошёл ассерт: `--output-on-failure` показывает
  ожидаемое/фактическое значение и `file:line` проверки
- **`***Timeout`** - тест завис: бесконечный цикл в теле теста **или
  в общем `TearDown`** фикстуры (`tests/test.hpp` вызывает `Clear`
  после каждого `StackTest.*`, поэтому «висеть» может и тест, который
  ты не менял). Скрипты в этом случае сами печатают подсказку
- скрипты останавливаются на **первой** ошибке (`--stop-on-failure`);
  чтобы увидеть все упавшие тесты разом: `cd build && ctest --output-on-failure`
- живой вывод: `./build/test_stack` - в терминале видно каждый
  `[ RUN ] ...`, последний напечатанный и есть зависший
- ASan-сборка при ошибке памяти печатает стек - читай верхние кадры,
  там файл/строка в твоём коде

## Сборка с ASan (обязательно прогонять чтобы поймать UB)

Перед сдачей обязаны быть зелёными **и** обычная сборка, **и** ASan-сборка.
Скрипт для ASan-сборки:

```bash
bash build_asan.sh
```

Вручную - те же шаги, добавляется один флаг:

```bash
cmake -B build-asan -DWITH_ASAN=ON
cmake --build build-asan
cd build-asan && ctest --output-on-failure
```

CMake сам делает всю платформенную работу
(на macOS для ctest автоматически выставляется `ASAN_OPTIONS=detect_leaks=1`).

Что ловит ASan-сборка:

| ОС      | ООБ / use-after-free / двойной free | Утечки памяти (LSan)            |
| ------- | ----------------------------------- | ------------------------------- |
| Linux   | да                                  | да (включён по умолчанию)       |
| macOS   | да                                  | да (включается ctest-ом)        |

Проверить, что ASan действительно активен:

```bash
ASAN_OPTIONS=verbosity=1 ./build-asan/test_stack --gtest_list_tests
```

В выводе должны быть строки `AddressSanitizer ... Init done`
(и на Linux - `LeakSanitizer: checking for leaks`).

## Troubleshooting

- **Первый configure висит/падает на скачивании** - нужен доступ в интернет
  (gtest тянется с github). После успешного configure сеть больше не нужна.
- **CMake старый** - `cmake --version` показывает < 3.16. Поставь новый:
  Linux - из репозитория дистрибутива или [cmake.org](https://cmake.org/download/),
  macOS - `brew upgrade cmake`.
- **`No rule to make target` / сломанная сборка** - удали папку сборки и
  запусти скрипт заново: `rm -rf build && bash build.sh`.
- **Тест «висит»** - скорее всего в реализации бесконечный цикл; ctest сам
  убьёт тест через 7 секунд, покажет его имя с `***Timeout`, а скрипт
  напечатает подсказку про возможный бесконечный цикл.
- **macOS: `clang: command not found`** - `xcode-select --install`.
- **macOS: ASan-отчёты без file:line** - собери ASan-сборку с
  `-DCMAKE_BUILD_TYPE=Debug` или прогони `dsymutil` по бинарнику.
- **`CMake Error: Unable to (re)create the private pkgRedirects directory`
  (или «фантомная» папка `build`, которая то есть, то нет)** - рассинхрон
  файловой системы WSL на `/mnt/c`. Удали `build`
  (`rm -rf build`), а если он «не удаляется/File exists» - перезапусти WSL
  из Windows (`wsl --shutdown`) и собери заново. Скрипт тоже подскажет это.
- **WSL** - работает как Linux: те же команды и скрипты, надо открыть папку с проектом и работать в ней (в нашем случае папка `stack`).
  Если на `/mnt/c` регулярно странности с файлами - перенеси проект в
  Linux-ФС: `cp -r /mnt/c/Users/<ты>/Downloads/stack ~/stack` и работай в `~/stack`.
- **Нативный Windows** - не поддерживается: собирай в WSL.
- **Полная очистка** - `rm -rf build build-asan`.
