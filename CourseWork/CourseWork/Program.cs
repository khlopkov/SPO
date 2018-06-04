using CourseWork.ThreadControllers;
using System;
using System.Diagnostics;
using System.Threading;

namespace CourseWork
{
    class Program
    {
        static void DrawHeader()
        {
            Console.WriteLine("┌──────────────────────┬──────────────┐");
            Console.WriteLine("│ Method               │ Time(ms)     │");
        }
        static void DrawLine(string methodName, long time)
        {
            Console.WriteLine("├──────────────────────┼──────────────┤");
            Console.Write("│ " + methodName);
            for (int i = 0; i < 21 - methodName.Length; i++)
            {
                Console.Write(" ");
            }
            Console.Write("│ " + time.ToString());
            for (int i = 0; i < 13 - time.ToString().Length; i++)
            {
                Console.Write(" ");
            }
            Console.WriteLine("│");
        }
        static void DrawFooter()
        {
            Console.WriteLine("└──────────────────────┴──────────────┘");
        }
        static void Main(string[] args)
        {
            int[] arr = GenerateArray(1000);
            Console.WriteLine("Please, input key to find in generated array");
            int key = Convert.ToInt32(Console.ReadLine());
            IThreadController<int> threadController = new EventController<int>();
            DrawHeader();
            DrawLine("Events", RunThread(threadController, arr, key));
            threadController = new SemaphoreController<int>();
            DrawLine("Semaphores", RunThread(threadController, arr, key));
            threadController = new MutexController<int>();
            DrawLine("Mutexes", RunThread(threadController, arr, key));
            threadController = new CriticalSectionController<int>();
            DrawLine("Critical sections", RunThread(threadController, arr, key));
            DrawFooter();
            Console.ReadLine();
        }
        private static long RunThread(IThreadController<int> controller, int[] arr, int key)
        {
            int[] arr1 = new int[arr.Length];
            Array.Copy(arr, arr1, arr1.Length);
            int[] arr2 = new int[arr.Length];
            Array.Copy(arr, arr2, arr1.Length);
            Thread threadMerge = new Thread(new ParameterizedThreadStart(controller.RunSortMerge));
            Thread threadShell = new Thread(new ParameterizedThreadStart(controller.RunSortShell));
            Thread threadFind = new Thread(new ParameterizedThreadStart(controller.RunFind));
            Stopwatch time = Stopwatch.StartNew();
            threadMerge.Start(arr1);
            threadFind.Start(key);
            threadShell.Start(arr2);
            threadFind.Join();
            threadShell.Join();
            threadMerge.Join();
            time.Stop();
            return time.ElapsedMilliseconds;

        }
        private static int[] GenerateArray(int n)
        {
            Random rnd = new Random();
            int[] arr = new int[n];
            for (int i = 0; i < n; i++)
            {
                arr[i] = rnd.Next(10000);
            }
            return arr;
        }
    }
}
