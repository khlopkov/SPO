using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace CourseWork
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.BufferHeight = 10000;
            ThreadController u = new CourseWork.ThreadController();
            int[] arr1 = generateArr(1000);
            int[] arr2 = new int[arr1.Length];
            Array.Copy(arr1, arr2, arr1.Length);
            int key = 100;
            Thread sortMergeThread = new Thread(new ParameterizedThreadStart(u.RunSortMerge));
            sortMergeThread.Name = "Merge sort thread";
            Thread sortShellThread = new Thread(new ParameterizedThreadStart(u.RunSortShell));
            sortShellThread.Name = "Shell's sort thread";
            Thread findThread = new Thread(new ParameterizedThreadStart(u.RunFind));
            findThread.Name = "find";
            sortMergeThread.Start(arr1);
            sortShellThread.Start(arr2);
            findThread.Start(key);
            Console.ReadLine();
        }
        private static int[] generateArr(int n)
        {
            Random rnd = new Random();
            int[] arr = new int[n];
            for (int i = 0; i < n; i++)
            {
                arr[i] = rnd.Next(1000);
            }
            return arr;
        }
    }
}
