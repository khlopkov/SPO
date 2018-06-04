using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CourseWork.ThreadControllers
{
    class SemaphoreController<T> : IThreadController<T> where T : System.IComparable<T>
    {
        Semaphore waitChangeHandler = new Semaphore(1,1);
        volatile bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        volatile T[] m1;
        AutoResetEvent findEvent = new AutoResetEvent(false);
        public SemaphoreController()
        {
            sorter = new Sorter<T>(this);
            finder = new Finder<T>();
        }

        public void RunFind(object obj)
        {
            T key = (T)obj;
            Thread.Sleep(2);
            while (firstSorting || secondSorting)
            {
                findEvent.WaitOne();
                waitChangeHandler.WaitOne();
                try
                {
                    T foundKey = finder.Find(key, this.m1);
                }
                catch (KeyNotFoundException e)
                {
                }
                waitChangeHandler.Release();
            }
        }
        public void Change(T[] arr)
        {
            
            waitChangeHandler.WaitOne();
            if (m1 == null)
                m1 = new T[arr.Length];
            Array.Copy(arr, m1, arr.Length);
            waitChangeHandler.Release();
            findEvent.Set();
        }

        public void RunSortMerge(object obj)
        {
            T[] arr = (T[])obj;
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
