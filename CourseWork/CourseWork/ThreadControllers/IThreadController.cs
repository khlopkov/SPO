using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CourseWork.ThreadControllers
{
    interface IThreadController<T> where T:IComparable<T>
    {
        void RunFind(object obj);
        void Change(T[] arr);
        void RunSortMerge(object obj);
        void RunSortShell(object obj);
    }
}
