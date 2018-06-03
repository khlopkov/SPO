using System;
using System.Threading;

namespace CourseWork
{
    class ThreadController
    {
        static AutoResetEvent waitChangeHandler = new AutoResetEvent(false);
        static AutoResetEvent waitFindHandler = new AutoResetEvent(true);
        bool firstSorting = true, secondSorting = true;
        private Sorter sorter;
        private Finder finder;
        public ThreadController()
        {
            sorter = new Sorter(this);
            finder = new Finder(this);
        }
        int[] m1;
        public void RunFind(object obj)
        {
            int key = (int)obj;
            while (firstSorting || secondSorting) {
                waitChangeHandler.WaitOne();
                waitChangeHandler.Reset();
                Console.Write("Trying to find key... ");
                int foundKey = finder.Find(key, this.m1);
                if (foundKey != -1)
                {
                    Console.WriteLine("Key has found");
                }
                else
                {
                    Console.WriteLine("Key hasn't found");
                }
                waitFindHandler.Set();
            }
        }
        public void Change(int[] arr)
        {
            waitFindHandler.WaitOne();
            waitFindHandler.Reset();
            Console.WriteLine("Changing array by " + Thread.CurrentThread.Name);
            m1 = new int[arr.Length];
            Array.Copy(arr, m1, arr.Length);
            waitChangeHandler.Set();
        }

        private void merge(int[] arr, int left, int mid, int right)
        {
            int it1 = 0, it2 = 0;
            int[] result = new int[right - left];
            while ((left + it1 < mid) && (mid + it2 < right))
            {
                if (arr[left + it1] < arr[mid + it2])
                {
                    result[it1 + it2] = arr[left + it1];
                    it1 += 1;
                }else
                {
                    result[it1 + it2] = arr[mid + it2];
                    it2 += 1;
                }
            }
            while (left + it1 < mid)
            {
                result[it1 + it2] = arr[left + it1];
                it1 += 1;
            }
            while (mid + it2 < right)
            {
                result[it1 + it2] = arr[mid + it2];
                it2 += 1;
            }
            for (int i = 0; i < it1 + it2; i++)
                arr[left + i] = result[i];
        }

        public void RunSortMerge(object obj)
        {
            int[] arr = (int[]) obj;
            sorter.SortMerge(arr);
            firstSorting = false;
        }

        public void RunSortShell(object obj)
        {
            int[] arr = (int[])obj;
            sorter.SortShell(arr);
            secondSorting = false;
        }
    }
}
