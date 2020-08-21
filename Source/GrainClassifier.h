#pragma once

#include "doctest.h"
#include "../dependencies/Gist/src/Gist.h"

#include "Grain.h"

namespace Palette 
{
    /*
     * The PreAllocator allocator lets us instantiate a std::vector which points
     * to preallocated memory. 
     * 
     * Each Grain holds data of the type juce::AudioBuffer which makes 
     * interfacing with JUCE library code simpler as well as providing the
     * utilities of the type. However, every method in the Gist library takes a
     * std::vector. For performance reasons, we would like to use Gist methods
     * without reallocating all the memory in each juce::AudioBuffer so we need
     * a custom allocator.
     */
    template <typename T>
    class PreAllocator
    {
    private:
        T* memory_ptr;
        std::size_t memory_size;

    public:
        typedef std::size_t size_type;
        typedef T* pointer;
        typedef T value_type;

        PreAllocator(T* memory_ptr, std::size_t memory_size) : memory_ptr(memory_ptr), memory_size(memory_size) {}

        PreAllocator(const PreAllocator& other) throw() : memory_ptr(other.memory_ptr), memory_size(other.memory_size) {};

        template<typename U>
        PreAllocator(const PreAllocator<U>& other) throw() : memory_ptr(other.memory_ptr), memory_size(other.memory_size) {};

        template<typename U>
        PreAllocator& operator = (const PreAllocator<U>& other) { return *this; }
        PreAllocator<T>& operator = (const PreAllocator& other) { return *this; }

        // Note that we don't free memory on destruction since this vector does
        // not own the memory it refers to.
        ~PreAllocator() {}

        pointer allocate(size_type n, const void* hint = 0) { return memory_ptr; }
        void deallocate(T* ptr, size_type n) {}

        size_type max_size() const { return memory_size; }
    };

	template<typename SampleType>
	class GrainClassifier
	{
	public:
		/*
		 * GrainClassifier is a class which helps call methods in the Gist library.
		 * GrainClassifier fits the API to our application more conviently.
		 * frameSize is the size a grain in samples i.e. 441 samples for a
		 * 10 ms grain at 44.1 kHz sample rate.
		 */
		GrainClassifier(int frameSize, int sampleRate) : gist(frameSize, sampleRate) { }

		const auto spectralCentroid(const Grain<SampleType>& grain) 
        { 
            /*
             * Construct a std::vector from the interal pointer in the 
             * juce::AudioBuffer from within grain. This saves us from copying
             * an entire grain worth of data (which would otherwise be copied 
             * for each grain in the entire synth).
             */
            
            return gist.spectralCentroid(nonOwningVector(grain));
        }
	private:
		Gist<SampleType> gist;

        static const auto nonOwningVector = [](const Grain& grain)
        {
            return std::vector<SampleType, PreAllocator<SampleType>>(grain.sampleData.getReadPointer(0),
                grain.sampleData.getNumSamples());
        };
	};
}

TEST_CASE("Palette::PreAllocator<SampleType>(SampleType*, std::size_t)")
{

}