using System;
using System.Collections.Generic;
using System.Threading;

namespace CourseWork
{
    class ThreadController<T> where T : System.IComparable<T>
    {
        static AutoResetEvent waitChangeHandler = new AutoResetEvent(false);
        static AutoResetEvent waitFindHandler = new AutoResetEvent(true);
        bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        T[] m1;
        public ThreadController()
        {
            sorter = new Sorter<T>(this);
            finder = new Finder<T>(this);
        }
        
        public void RunFind(object obj)
        {
            T key = (T)obj;
            while (firstSorting || secondSorting) {
                waitChangeHandler.WaitOne();
                waitChangeHandler.Reset();
                Console.Write("Trying to find key... ");
                try
                {
                    T foundKey = finder.Find(key, this.m1);
                    Console.WriteLine("Key has found");
                }
                catch (KeyNotFoundException e)
                {
                    Console.WriteLine("Key hasn't found");
                }
                waitFindHandler.Set();
            }
        }
        public void Change(T[] arr)
        {
            waitFindHandler.WaitOne();
            waitFindHandler.Reset();
            Console.WriteLine("Changing array by " + Thread.CurrentThread.Name);
            m1 = new T[arr.Length];
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
            T[] arr = (T[]) obj;
            sorter.SortMerge(arr);
            firstSorting = false;
        }

        public void RunSortShell(object obj)
        {
            T[] arr = (T[])obj;
            sorter.SortShell(arr);
            secondSorting = false;
        }
    }
}
