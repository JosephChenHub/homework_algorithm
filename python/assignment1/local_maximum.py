#!/usr/bin/env python

class Solution(object):
	#one solution
	def local_maximum(self,arr, start, end):
		if (end - start) <= 1:
			return max(arr[start], arr[end])

		mid = (start + end)/2
		left = self.local_maximum(arr, start, mid)
		right = self.local_maximum(arr, mid, end)

		return max(left, right)

	def findPeak(self, nums):
#		num = self.local_maximum(nums, 0, len(nums)-1)
#		return nums.index(num)
#		num = self.local_max(nums, 0, len(nums)-1)

		num = self.local_max3(nums, 0, len(nums)-1)
		return num

	#another 
	def local_max(self, arr, start, end):
		if (end - start) <= 1:
			if arr[start] <= arr[end]:
				return end
			else:
				return start
		mid = (start + end)/2
		left = self.local_max(arr, start, mid)
		right = self.local_max(arr, mid, end)
		
		if arr[left] > arr[right]:
			return left
		else:
			return right

	#third 
	def local_max3(self, arr, start, end):
		if (end - start) <= 1:
			if arr[start] <= arr[end]:	
				return end
			else:
				return start
		mid = (start + end)/2
		if arr[mid] > arr[mid-1] and arr[mid] > arr[mid + 1]:
			return mid
		elif arr[mid] < arr[mid-1]:
			return self.local_max3(arr, start, mid-1)
		else:
			return self.local_max3(arr, mid+1,  end)


a = [9, 6, 3, 14, 5, 7, 4]
s = Solution()
num = s.findPeak(a)
print('local maximum num:{}'.format(num))

