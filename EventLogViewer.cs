using System;
using System.Diagnostics;

namespace EventLogViewer
{
    class Program
    {
        static void Main(string[] args)
        {
            bool exit = false;

            while (!exit)
            {
                Console.Clear();
                Console.WriteLine("Доступные журналы событий:");
                Console.WriteLine("1. Система");
                Console.WriteLine("2. Безопасность");
                Console.WriteLine("3. Приложения");
                Console.WriteLine();

                Console.Write("Выберите номер журнала для просмотра: ");
                string input = Console.ReadLine();

                if (int.TryParse(input, out int selectedEventLogIndex))
                {
                    switch (selectedEventLogIndex)
                    {
                        case -1:
                            exit = true;
                            break;
                        case 1:
                            EventLog selectedEventLog1 = SelectEventLog("System");
                            if (selectedEventLog1 != null)
                            {
                                Console.WriteLine($"События из журнала: {selectedEventLog1.LogDisplayName}");
                                DisplayEventLogEntries(selectedEventLog1);
                                WaitForMenuInput();
                            }
                            break;
                        case 2:
                            EventLog selectedEventLog2 = SelectEventLog("Security");
                            if (selectedEventLog2 != null)
                            {
                                Console.WriteLine($"События из журнала: {selectedEventLog2.LogDisplayName}");
                                DisplayEventLogEntries(selectedEventLog2);
                                WaitForMenuInput();
                            }
                            break;
                        case 3:
                            EventLog selectedEventLog3 = SelectEventLog("Application");
                            if (selectedEventLog3 != null)
                            {
                                Console.WriteLine($"События из журнала: {selectedEventLog3.LogDisplayName}");
                                DisplayEventLogEntries(selectedEventLog3);
                                WaitForMenuInput();
                            }
                            break;
                        default:
                            Console.WriteLine("Неверный номер журнала.");
                            WaitForMenuInput();
                            break;
                    }
                }
                else
                {
                    Console.WriteLine("Неверный ввод. Введите число.");
                    WaitForMenuInput();
                }
            }
        }

        static EventLog SelectEventLog(string logName)
        {
            try
            {
                EventLog eventLog = new EventLog(logName);
                return eventLog;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при выборе журнала событий '{logName}': {ex.Message}");
                return null;
            }
        }

        static void DisplayEventLogEntries(EventLog eventLog)
        {
            if (eventLog == null)
            {
                Console.WriteLine("Журнал событий недоступен.");
                return;
            }

            try
            {
                // Фильтр по имени
                Console.Write("Введите имя для фильтрации (оставьте пустым, чтобы не использовать фильтр): ");
                string nameFilter = Console.ReadLine();

                // Фильтр по дате
                Console.Write("Введите дату для фильтрации (в формате дд.мм.гггг, оставьте пустым, чтобы не использовать фильтр): ");
                string dateFilterString = Console.ReadLine();
                DateTime dateFilter;
                bool useDateFilter = DateTime.TryParse(dateFilterString, out dateFilter);

                foreach (EventLogEntry entry in eventLog.Entries)
                {
                    // Фильтрация по имени
                    if (!string.IsNullOrEmpty(nameFilter) && !entry.Source.Contains(nameFilter))
                    {
                        continue; // Пропускаем запись, если она не соответствует фильтру по имени
                    }

                    // Фильтрация по дате
                    if (useDateFilter && entry.TimeGenerated.Date != dateFilter.Date)
                    {
                        continue; // Пропускаем запись, если она не соответствует фильтру по дате
                    }

                    Console.WriteLine($"Источник: {entry.Source}");
                    Console.WriteLine($"Дата и время: {entry.TimeGenerated}");
                    Console.WriteLine($"Сообщение: {entry.Message}");
                    Console.WriteLine();

                    // Проверяем, была ли нажата клавиша для возврата в меню
                    if (Console.KeyAvailable)
                    {
                        ConsoleKeyInfo keyInfo = Console.ReadKey(true);
                        if (keyInfo.Key == ConsoleKey.Escape)
                        {
                            break; // Прерываем цикл и возвращаемся в меню
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при отображении событий: {ex.Message}");
            }
        }

        static void WaitForMenuInput()
        {
            Console.WriteLine("Нажмите любую клавишу для продолжения...");
            Console.ReadKey();
        }
    }
}