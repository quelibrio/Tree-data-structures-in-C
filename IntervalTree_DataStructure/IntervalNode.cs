using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Telerik.BlackDragon.Client.Markers.DataStructures
{
	/// <summary>
	/// The Node class contains the interval tree information for one single node
	/// </summary>
	public class IntervalNode<T, D>
		where T : IIntervalProvider<D>
		where D : struct, IComparable<D>
	{
		private readonly Dictionary<Interval<D>, List<T>> intervals;
		private Interval<D> totalInterval;
		private D center;
		private IntervalNode<T, D> leftNode;
		private IntervalNode<T, D> rightNode;

		public IntervalNode()
		{
			intervals = new Dictionary<Interval<D>, List<T>>();
			center = default(D);
			leftNode = null;
			rightNode = null;
		}

		private string debug
		{
			get
			{
				StringBuilder sb = new StringBuilder();
				foreach (var key in intervals.Keys)
				{
					sb.AppendLine(key.ToString());
					sb.AppendLine("==");
					foreach (var val in intervals[key])
					{
						sb.AppendLine(val.ToString());
						sb.AppendLine("--");
					}

					sb.AppendLine("***");
				}

				return sb.ToString();
			}
		}

		public IntervalNode(IEnumerable<T> intervalDataList)
		{
			intervals = new Dictionary<Interval<D>, List<T>>();

			var endpoints = new List<D>();

			foreach (var intervalData in intervalDataList)
			{
				endpoints.Add(intervalData.Interval.Start);
				endpoints.Add(intervalData.Interval.End);
			}
			endpoints.Sort();

			Nullable<D> median = GetMedian(endpoints);
			center = median.GetValueOrDefault();

			var left = new List<T>();
			var right = new List<T>();

			foreach (var intervalData in intervalDataList)
			{
				if (intervalData.Interval.End.CompareTo(center) < 0)
				{
					left.Add(intervalData);
				}
				else if (intervalData.Interval.Start.CompareTo(center) > 0)
				{
					right.Add(intervalData);
				}			
				else
				{
					List<T> list;
					if (!intervals.TryGetValue(intervalData.Interval, out list))
					{
						intervals.Add(intervalData.Interval, new List<T>());
					}
					if (totalInterval == null)
					{
						totalInterval = intervalData.Interval;
					}
					else
					{
						if (totalInterval.Start.CompareTo(intervalData.Interval.Start) > 0)
						{
							totalInterval.Start = intervalData.Interval.Start;
						}
						if (totalInterval.End.CompareTo(intervalData.Interval.End) < 0)
						{
							totalInterval.End = intervalData.Interval.End;
						}
					}
					intervals[intervalData.Interval].Add(intervalData);					
				}
			}

			if (left.Count > 0)
			{
				leftNode = new IntervalNode<T, D>(left);
			}
			if (right.Count > 0)
			{
				rightNode = new IntervalNode<T, D>(right);
			}
		}

		/// <summary>
		/// Perform a stabbing Query on the node
		/// </summary>
		/// <param name="time">the time to Query at</param>
		/// <returns>all stubedIntervals containing time</returns>
		public List<T> Stab(D time, ContainConstrains constraint)
		{
			List<T> result = new List<T>();

			if (totalInterval != null && totalInterval.Contains(time, constraint))
			{
				foreach (var entry in intervals)
				{
					if (entry.Key.Contains(time, constraint))
					{
						result.AddRange(entry.Value);
					}
				}
			}

			if (time.CompareTo(center) < 0 && leftNode != null)
			{
				result.AddRange(leftNode.Stab(time, constraint));
			}
			else if (time.CompareTo(center) > 0 && rightNode != null)
			{
				result.AddRange(rightNode.Stab(time, constraint));
			}
			return result;
		}

		/// <summary>
		/// Perform an interval intersection Query on the node
		/// </summary>
		/// <param name="target">the interval to intersect</param>
		/// <returns>all stubedIntervals containing time</returns>
		public List<T> Query(Interval<D> target)
		{
			List<T> result = new List<T>();

			if (totalInterval != null && totalInterval.Intersects(target))
			{
				foreach (var entry in intervals)
				{
					if (entry.Key.Intersects(target))
					{
						result.AddRange(entry.Value);
					}
				}
			}

			if (target.Start.CompareTo(center) < 0 && leftNode != null)
			{
				result.AddRange(leftNode.Query(target));
			}
			if (target.End.CompareTo(center) > 0 && rightNode != null)
			{
				result.AddRange(rightNode.Query(target));
			}
			return result;
		}

		public D Center
		{
			get
			{
				return center;
			}
			set
			{
				center = value;
			}
		}

		public IntervalNode<T, D> Left
		{
			get
			{
				return leftNode;
			}
			set
			{
				leftNode = value;
			}
		}

		public IntervalNode<T, D> Right
		{
			get
			{
				return rightNode;
			}
			set
			{
				rightNode = value;
			}
		}

		/// <summary>
		/// the median of the set, not interpolated
		/// </summary>
		/// <param name="set"></param>
		/// <returns></returns>
		private Nullable<D> GetMedian(List<D> set)
		{
			int i = 0;
			int middle = set.Count / 2;
			foreach (D point in set)
			{
				if (i == middle)
				{
					return point;
				}
				i++;
			}
			return null;
		}

		public override string ToString()
		{
			var sb = new StringBuilder();
			sb.Append(center + ": ");
			foreach (var entry in intervals)
			{
				sb.Append("[" + entry.Key.Start + "," + entry.Key.End + "]:{");
				foreach (T data in entry.Value)
				{
					sb.Append("(" + data + ")");
				}
				sb.Append("} ");
			}
			return sb.ToString();
		}
	}
}