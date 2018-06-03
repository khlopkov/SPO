using System;
using System.Threading;

namespace CourseWork
{
    class Program
    {
        static void PrintMenu()
        {
            Console.WriteLine("1. Show generated array");
            Console.WriteLine("2. Run sorting");
        }
        static void PrintArray(int[] arr)
        {
            foreach (int elem in arr)
            {
                Console.Write(elem);
                Console.Write(" ");
            }
        }
        static void RunMultiThreading(int[] arr, int key)
        {
            ThreadController<int> u = new CourseWork.ThreadController<int>();
            int[] arr1 = arr;
            int[] arr2 = new int[arr1.Length];
            Array.Copy(arr1, arr2, arr1.Length);
            Thread sortMergeThread = new Thread(new ParameterizedThreadStart(u.RunSortMerge));
            sortMergeThread.Name = "Merge sort thread";
            Thread sortShellThread = new Thread(new ParameterizedThreadStart(u.RunSortShell));
            sortShellThread.Name = "Shell's sort thread";
            Thread findThread = new Thread(new ParameterizedThreadStart(u.RunFind));
            findThread.Name = "find";
            sortMergeThread.Start(arr1);
            sortShellThread.Start(arr2);
            findThread.Start(key);
            sortMergeThread.Join();
            sortShellThread.Join();
            findThread.Join();
        }
        static void Main(string[] args)
        {
            Console.BufferHeight = 10000;
            int[] arr = generateArr(1000);
            int key = 0;
            int choosedItem = 0;
            while (choosedItem != 2)
            {
                PrintMenu();
                try
                {
                    choosedItem = Convert.ToInt16(Console.ReadLine());
                }
                catch(FormatException e)
                {
                    Console.WriteLine("Incorrect item");
                    continue;
                }
                if (choosedItem == 1)
                {
                    PrintArray(arr);
                }
                if (choosedItem == 2)
                {
                    while (true)
                    {
                        Console.WriteLine("Input key, which you want to find in array");
                        try
                        {
                            key = Convert.ToInt16(Console.ReadLine());
                            break;
                        }
                        catch(FormatException e)
                        {
                            Console.WriteLine("Incorrect key");
                        }
                    }
                    RunMultiThreading(arr, key);
                }
            }
            Console.WriteLine("Press enter to print sorted array");
            Console.ReadLine();
            PrintArray(arr);
            Console.ReadLine();
        }
        private static int[] generateArr(int n)
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
