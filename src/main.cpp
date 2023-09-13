

#include <__fwd/string_view.h>
#include <array>
#include <bit>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <streambuf>
#include <string_view>
#include <immintrin.h>

using v4 [[gnu::vector_size(32)]] = uint16_t;
using v4a [[gnu::vector_size(32)]] = uint32_t;
using v4b [[gnu::vector_size(4)]] = uint8_t;
using v2b [[gnu::vector_size(2)]] = uint8_t;

constexpr uint32_t sso = std::string{}.capacity();

struct [[gnu::aligned(16)]] PCMHeaderRef
{
    uint32_t size_t;
    uint32_t SampleRate;
    uint32_t bytePlaybackrate : 16;
    uint32_t bitDepth : 4;
    uint32_t channels : 4;
    uint32_t samples;

   [[nodiscard]] auto durationSecs() -> uint64_t {return samples/SampleRate;}

};

// constexpr std::string_view fail = std::string_view{"FAIL!"};
// //This Function generates very ugly ASM (likely due to heap Allocations from std::string); May be worth optimising this later
// [[gnu::pure]] auto testDir(  std::string ax="pcm/") noexcept -> std::string_view
// {
//       // const std::filesystem::path png{".png"};
//       for (const auto& dir_entry : std::filesystem::directory_iterator{ax}->path().filename()) 
//       {
//           if(dir_entry.extension()==".wav"||".pcm")
//           {
//             return ax.append(dir_entry.string());
//           }
//       }
//       return fail;
// }


auto getu4Byte(const std::array<uint8_t, 64> &headerRef, uint8_t ref) -> uint32_t
{
    auto bx4 = v4b{headerRef[ref+0],headerRef[ref+1],headerRef[ref+2],headerRef[ref+3]};
    return (std::bit_cast<uint32_t>(bx4));
}

auto getu4String(const std::array<uint8_t, 64> &headerRef, uint8_t ref) -> std::u8string
{
 auto bx4 = std::u8string{headerRef[ref+0],headerRef[ref+1],headerRef[ref+2],headerRef[ref+3]};
    return (bx4);
}

auto getu2Byte(const std::array<uint8_t, 64> &headerRef, uint8_t ref) -> uint16_t
{
    auto bx2 = v2b{headerRef[ref+0],headerRef[ref+1]};
    return (std::bit_cast<uint16_t>(bx2));
}

auto extractPCMHeader(std::array<uint8_t, 64> headerRef) -> PCMHeaderRef
{
    uint32_t size = getu4Byte(headerRef,4);
    uint32_t AudioFormat = getu2Byte(headerRef,20);
    uint32_t numChannels = getu2Byte(headerRef,22);
    uint32_t samplerate = getu4Byte(headerRef,24);
    uint32_t byteRate = getu4Byte(headerRef,28);
    uint32_t blockAlign = getu4Byte(headerRef,32);
    uint16_t bitsPerSample = getu2Byte(headerRef,34);
    std::u8string Subchunk2data     = getu4String(headerRef,36);
    uint16_t Subchunk2Size     = getu4Byte(headerRef,40);
    std::u8string chunkID     = getu4String(headerRef,0);

    

    printf("--=RIFF Header Details:=--\n\n");
    printf("size: %i \n", size);
    printf("AudioFormat: %i \n", AudioFormat);
    printf("numChannels: %i \n", numChannels);
    printf("samplerate: %i \n", samplerate);
    printf("byteRate: %i bps\n", byteRate);
    printf("byteRate: %i kbps\n", byteRate>>10);
    printf("blockAlign: %i \n", blockAlign);
    printf("bitsPerSample: %i \n", bitsPerSample);
    printf("samples: %i \n", size/bitsPerSample);
    printf("%s \n", Subchunk2data.c_str());
    printf("Subchunk2Size: %i \n", Subchunk2Size);
    printf("%s \n", chunkID.c_str());
    return 
    {
        size,
    samplerate,
    bitsPerSample,
     byteRate,
    numChannels,
      size/bitsPerSample,
    };
}

//Align PCMArray to bitdepth

template<typename type>
auto pcm2Array(std::string_view imgDir = "tst.wav")
{

    FILE *f = fopen64(imgDir.data(), "rb");
    
    std::array<uint8_t, 64> PCMheader{};

    fread(PCMheader.data(), sizeof(uint8_t), PCMheader.size(), f);

    extractPCMHeader(PCMheader);



    // return std::to_array<type>() ;
}



auto main() -> int
{
    printf("sso size: %i \n", sso);
    pcm2Array<uint16_t>();

}