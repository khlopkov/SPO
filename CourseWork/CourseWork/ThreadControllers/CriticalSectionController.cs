using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CourseWork.ThreadControllers
{
    class CriticalSectionController<T> : IThreadController<T> where T : System.IComparable<T>
    {
        bool changingInitially = true;
        volatile bool firstSorting = true, secondSorting = true;
        private Sorter<T> sorter;
        private Finder<T> finder;
        volatile static object changeLocker = new object();
        AutoResetEvent findEvent = new AutoResetEvent(false);
        volatile T[]  m1;
        public CriticalSectionController()
        {
            sorter = new Sorter<T>(this);
            finder = new Finder<T>();
        }
        public void RunFind(object obj)
        {
            T key = (T)obj;
            while (firstSorting || secondSorting)
            {
                bool findingInitially = true;
                findEvent.WaitOne();
                lock (changeLocker)
                {
                    if (findingInitially)
                    {
                        Monitor.PulseAll(changeLocker);
                        findingInitially = false;
                    }
                    try
                    {
                        T foundKey = finder.Find(key, this.m1);

                    }
                    catch (KeyNotFoundException e)
                    {
                        
                    }
                    Monitor.PulseAll(changeLocker);
                }
                
            }
        }
        public void Change(T[] arr)
        {
            lock (changeLocker)
            {
                if (!changingInitially)
                    Monitor.Wait(changeLocker, 100);
                else changingInitially = false;
                if (m1 == null)
                    m1 = new T[arr.Length];
                Array.Copy(arr, m1, arr.Length);
                findEvent.Set();
            }
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

