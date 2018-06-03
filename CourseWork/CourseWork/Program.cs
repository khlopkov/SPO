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
            Console.WriteLine("├──────────────────────┼──────────────┤");
        }
        static void DrawLine(string methodName, int time)
        {

        }
        static void Main(string[] args)
        {
            int[] arr = GenerateArray(1000);
            IThreadController<int> threadController = new EventController<int>();
            Console.Write("Events: ");
            Console.WriteLine(RunThread(threadController, arr, 100));
            threadController = new SemaphoreController<int>();
            Console.Write("Semaphores: ");
            Console.WriteLine(RunThread(threadController, arr, 100));
            threadController = new MutexController<int>();
            Console.Write("Mutexes: ");
            Console.WriteLine(RunThread(threadController, arr, 100));
            threadController = new CriticalSectionController<int>();
            Console.Write("Critical sections: ");
            Console.WriteLine(RunThread(threadController, arr, 100));
            Console.ReadKey();
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
