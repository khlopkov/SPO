using System;
using System.Collections.Generic;
using System.Threading;

namespace CourseWork.ThreadControllers
{
    class MutexController<T> : IThreadController<T> where T : System.IComparable<T>
    {
        Mutex waitChangeHandler = new Mutex();
        volatile bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        volatile T[] m1;
        AutoResetEvent findEvent = new AutoResetEvent(false);
        public MutexController()
        {
            sorter = new Sorter<T>(this);
            finder = new Finder<T>();
        }

        public void RunFind(object obj)
        {
            T key = (T)obj;
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
                waitChangeHandler.ReleaseMutex();
            }
        }
        public void Change(T[] arr)
        {
            waitChangeHandler.WaitOne();
            if (m1 == null)
                m1 = new T[arr.Length];
            Array.Copy(arr, m1, arr.Length);
            waitChangeHandler.ReleaseMutex();
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
