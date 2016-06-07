using System;
using System.Collections.Generic;
using System.Text;

namespace Telerik.BlackDragon.Client.Markers.DataStructures
{
	/// <summary>
	/// An Interval Tree is essentially a map from stubedIntervals to objects, which
	/// can be queried for all data associated with a particular interval of time
	/// </summary>
	/// <typeparam name="T">Type of data store on each interval.</typeparam>
	/// <typeparam name="D">Type define the interval. Must be struct that implement IComperable</typeparam>
	/// <remarks>
	/// This code was translated from Java to C# by ido.ran@gmail.com from the web site http://www.thekevindolan.com/2010/02/interval-tree/index.html.
	/// </remarks>
	public class IntervalTree<T, D>
		where T : IIntervalProvider<D>
		where D : struct, IComparable<D>
	{

		private IntervalNode<T, D> head;
		private readonly IList<T> intervalList;
		private bool inSync;
		private int size;

		/// <summary>
		/// Instantiate a new interval tree with no stubedIntervals
		/// </summary>
		public IntervalTree()
		{
			this.head = new IntervalNode<T, D>();
			this.intervalList = new List<T>();
			this.inSync = true;
			this.size = 0;
		}

		/// <summary>
		/// Instantiate and Build an interval tree with a preset list of stubedIntervals
		/// </summary>
		/// <param name="intervalList">The list of stubedIntervals to use</param>
		public IntervalTree(IList<T> intervalList)
		{
			this.head = new IntervalNode<T, D>(intervalList);
			this.intervalList = intervalList;
			this.inSync = true;
			this.size = intervalList.Count;
		}

		/// <summary>
		/// Perform a stabbing Query, returning the associated data.
		/// Will rebuild the tree if out of sync
		/// </summary>
		/// <param name="time">The time to Stab</param>
		/// <returns>The data associated with all stubedIntervals that contain time</returns>
		public List<T> Get(D time)
		{
			return Get(time, StabMode.Contains);
		}

		public List<T> Get(D time, StabMode mode)
		{
			Build();

			List<T> stubedIntervals;

			switch (mode)
			{
				case StabMode.Contains:
					stubedIntervals = head.Stab(time, ContainConstrains.None);
					break;
				case StabMode.ContainsStart:
					stubedIntervals = head.Stab(time, ContainConstrains.IncludeStart);
					break;
				case StabMode.ContainsStartThenEnd:
					stubedIntervals = head.Stab(time, ContainConstrains.IncludeStart);
					if (stubedIntervals.Count == 0)
					{
						stubedIntervals = head.Stab(time, ContainConstrains.IncludeEnd);
					}
					break;
				default:
					throw new ArgumentException("Invalid StubMode " + mode, "mode");
			}

			return stubedIntervals;
		}

		/// <summary>
		/// Perform an interval Query, returning the associated data.
		/// Will rebuild the tree if out of sync.
		/// </summary>
		/// <param name="start">the start of the interval to check</param>
		/// <param name="end">end of the interval to check</param>
		/// <returns>the data associated with all stubedIntervals that intersect target</returns>
		public List<T> Get(D start, D end)
		{
			Build();
			return head.Query(new Interval<D>(start, end));
		}

		

		/// <summary>
		/// Determine whether this interval tree is currently a reflection of all stubedIntervals in the interval list
		/// </summary>
		/// <returns>true if no changes have been made since the last Build</returns>
		public bool IsInSync()
		{
			return inSync;
		}

		/// <summary>
		/// Build the interval tree to reflect the list of stubedIntervals.
		/// Will not run if this is currently in sync
		/// </summary>
		/// </summary>
		public void Build()
		{
			if (!inSync)
			{
				head = new IntervalNode<T, D>(intervalList);
				inSync = true;
				size = intervalList.Count;
			}
		}

		/// <summary>
		/// Get the number of entries in the currently built interval tree
		/// </summary>
		public int CurrentSize
		{
			get
			{
				return size;
			}
		}

		/// <summary>
		/// Get the number of entries in the interval list, equal to .size() if inSync()
		/// </summary>
		public int ListSize
		{
			get
			{
				return intervalList.Count;
			}
		}


		/// <summary>
		/// Get all the stubedIntervals in this tree.
		/// This method does not build the tree.
		/// </summary>
		public IList<T> Intervals
		{
			get
			{
				return intervalList;
			}
		}

		public override String ToString()
		{
			return NodeString(head, 0);
		}

		private String NodeString(IntervalNode<T, D> node, int level)
		{
			if (node == null)
				return "";

			var sb = new StringBuilder();
			for (int i = 0; i < level; i++)
				sb.Append("\t");
			sb.Append(node + "\n");
			sb.Append(NodeString(node.Left, level + 1));
			sb.Append(NodeString(node.Right, level + 1));
			return sb.ToString();
		}
	}

	public enum StabMode
	{
		Contains,
		ContainsStart,
		ContainsStartThenEnd
	}
}
