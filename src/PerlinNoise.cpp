#include "PerlinNoise.h"
#include <numeric>
#include <random>

PerlinNoise::PerlinNoise(std::string seedString)
{
  for (char c : seedString)
    _seed += static_cast<uint32>(c);
  _seed = 123123;
  initializeHashTableTable();
}

void PerlinNoise::initializeHashTableTable()
{
  std::vector<int> numbers(256);
  std::iota(numbers.begin(), numbers.end(), 0); // Fill with numbers from 0 to 255

  std::default_random_engine engine(_seed);

  for (int i = 0; i < 256; i++) 
  {
    std::uniform_int_distribution<int> dist(0, numbers.size() - 1);
    int randomIndex = dist(engine);
    _permutation[i] = numbers[randomIndex];
    numbers.erase(numbers.begin() + randomIndex);
  }
  for (int x = 0; x < 512; x++) {
    _hashTable[x] = _permutation[x % 256];
  }
}

inline double PerlinNoise::getGradient(int32 hash, double x, double y, double z)
{
  switch(hash & 0xF)
  {
    case 0x0: return  x + y;
    case 0x1: return -x + y;
    case 0x2: return  x - y;
    case 0x3: return -x - y;
    case 0x4: return  x + z;
    case 0x5: return -x + z;
    case 0x6: return  x - z;
    case 0x7: return -x - z;
    case 0x8: return  y + z;
    case 0x9: return -y + z;
    case 0xA: return  y - z;
    case 0xB: return -y - z;
    case 0xC: return  y + x;
    case 0xD: return -y + z;
    case 0xE: return  y - x;
    case 0xF: return -y - z;
    default: return 0; // never happens
  }
}

double PerlinNoise::generatePerlin(double x, double y, double z)
{
  int32 xi = static_cast<int32>(x) & 255;
  int32 yi = static_cast<int32>(y) & 255;
  int32 zi = static_cast<int32>(z) & 255;
  double xf = x - static_cast<int32>(x);
  double yf = y - static_cast<int32>(y);
  double zf = z - static_cast<int32>(z);
  double u = fade(xf);
  double v = fade(yf);
  double w = fade(zf);

  int32 aaa, aba, aab, abb, baa, bba, bab, bbb;
  aaa = _hashTable[_hashTable[_hashTable[xi] + yi] + zi];
  aba = _hashTable[_hashTable[_hashTable[xi] + yi+1] + zi];
  aab = _hashTable[_hashTable[_hashTable[xi] + yi] + zi+1];
  abb = _hashTable[_hashTable[_hashTable[xi] + yi+1] + zi+1];
  baa = _hashTable[_hashTable[_hashTable[xi+1] + yi] + zi];
  bba = _hashTable[_hashTable[_hashTable[xi+1] + yi+1] + zi];
  bab = _hashTable[_hashTable[_hashTable[xi+1] + yi] + zi+1];
  bbb = _hashTable[_hashTable[_hashTable[xi+1] + yi+1] + zi+1];

  double x1,x2,y1,y2;
  x1 = lerp(getGradient(aaa, xf,yf,zf), getGradient(baa, xf-1, yf,zf), u);
  x2 = lerp(getGradient(aba,xf,yf-1,zf), getGradient(bba,xf-1,yf-1,zf), u);
  y1 = lerp(x1,x2,v);

  x1 = lerp(getGradient(aab, xf, yf, zf-1), getGradient(bab, xf-1,yf, zf-1), u);
  x2 = lerp(getGradient(abb, xf,yf-1, zf-1), getGradient(bbb, xf-1, yf-1, zf-1), u);
  y2 = lerp(x1,x2,v);
//010 000 0-10

  return (lerp(y1,y2,w)+1)/2;
}

double PerlinNoise::generateOctavePerlin(double x, double y, double z, uint32 octaves, double persistence)
{
  double total = 0;
  double frequency = 1;
  double amplitude = 1;
  double maxValue = 0;

  for (uint32 i = 0; i < octaves; ++i)
  {
    total += generatePerlin(x * frequency, y * frequency, z * frequency);
    maxValue += amplitude;
    amplitude *= persistence;
    frequency *= 2;
  }
  return total/maxValue;
}