using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InheritanceTest
{
    public class Program
    {
        static void Main(string[] args)
        {
            ICollection results = new LinkedList<bool>();

            IInterface<IList> iint = new Extra<IList, IEnumerable, double>  ();
            var x = iint.IsTrue;

            IInterface<IReadOnlyCollection<int>> IBaseTwo = new BaseTwoClass<IReadOnlyCollection<int>>();
            var p = IBaseTwo.IsTrue;

            BaseClass<ICollection> baseClassExtra = new Extra<ICollection, IEnumerable, double>();
            var y = baseClassExtra.IsTrue; 

            Super<IDictionary,int> superExtra = new Extra<IDictionary, IEnumerable, int> ();
            var z = superExtra.IsTrue;

            Extra<IQueryable, IEnumerable, IQueryProvider> extra = new Extra<IQueryable,IEnumerable, IQueryProvider>();
            var q = extra.IsTrue;

            BaseTwoClass<ICollection> baseTwoSuper = new SuperTwo<ICollection, IEnumerable>();
            var l = baseTwoSuper.IsTrue;

            SuperTwo<ICollection, IEnumerable> superTwo = new SuperTwo<ICollection, IEnumerable>();
            var m = superTwo.IsTrue;

            Console.WriteLine();
        }    
    }

    internal interface IInterface<T>
    {
        bool TrueMethod();
        bool IsTrue { get;  }
    }
    
    internal abstract class BaseClass<T> : IInterface<T>
    {
        public abstract bool TrueMethod();

        public virtual bool IsTrue { get { return false; } }
    }

    internal abstract class Super<T, K> : BaseClass<T>
    {
        //Wow it hides upper level definition
        public new bool IsTrue { get { return false; } }
    }

    internal class BaseTwoClass<T> : IInterface<T>
    {
        public bool TrueMethod(){ return false; }

        public virtual bool IsTrue { get { return false; } }
    }

    internal class SuperTwo<T,K> : BaseTwoClass<T>
    {
        public override bool IsTrue{ get { return true; } }
    }

    internal class Extra<T,F,K> : Super<T,K> 
        where  T : F
        //where F : new() 
    {

        bool IsFalse(out IList list)
        {
            list = new ArrayList();
            list.Add(1);
            return false;
        }

        bool IsFalse1(ref IList list)
        {
            if (list.Count == 0)
            {
                list = new ArrayList();
                list.Add(2);
            }
            return false;
        }

        public bool Is { get; set; }

        public override bool TrueMethod(){return true;}

        private F data;
    }

}
