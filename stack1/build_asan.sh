#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

echo "=== Сборка с ASan ==="
if ! cmake -B build-asan -DWITH_ASAN=ON; then
  echo
  echo "❌ Конфигурация CMake не удалась."
  echo "Подсказка: удали папку сборки (rm -rf build-asan) и запусти скрипт заново."
  echo "Если ошибка про файловую систему/9p — перезапусти WSL из Windows: wsl --shutdown"
  exit 1
fi
cmake --build build-asan

echo
echo "=== Тесты (ASan) ==="

run_tests() {
  if ( cd build-asan && ctest --output-on-failure --stop-on-failure --output-log ctest_last.log "$@" ); then
    echo
    echo "✅ Все тесты прошли"
  else
    echo
    echo "❌ Тесты не прошли (см. вывод выше)"
    if grep -q "Timeout" build-asan/ctest_last.log 2>/dev/null; then
      echo "Подсказка: тест убит по таймауту — в коде вероятен бесконечный цикл (например, Clear/TearDown)."
    fi
    exit 1
  fi
}

if [ $# -gt 0 ]; then
  run_tests -R "$1" --no-tests=error
else
  run_tests
fi
