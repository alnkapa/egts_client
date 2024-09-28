# Эмулятор клиента для протокола EGTS

## Описание
Эмулятор клиента для протокола EGTS предназначен для тестирования и отладки систем, использующих данный протокол. Проект позволяет эмулировать поведение клиента, отправляющего данные на сервер, и получать ответы.

## Поддерживаемые сервисы и подзаписи

- **EGTS_AUTH_SERVICE**
  - `EGTS_SR_RECORD_RESPONSE`
  - `EGTS_SR_TERM_IDENTITY`
  - `EGTS_SR_MODULE_DATA`

- **EGTS_TELEDATA_SERVICE**
  - `EGTS_SR_POS_DATA`
  - `EGTS_SR_EXT_POS_DATA`

- **EGTS_COMMANDS_SERVICE**
  - `EGTS_SR_COMMAND_DATA`

  
## Сборка

- mkdir -p build
- cd build
- cmake ..
- cmake --build .

## Тесты

- mkdir -p build
- cd build
- cmake -DBUILD_TESTING=ON ..
- cmake --build .
- ctest -rerun-failed --output-on-failure

## Запуск

- mkdir -p build
- cd build
- cmake ..
- cmake --build .
- cd ..
- ./build/egts_client