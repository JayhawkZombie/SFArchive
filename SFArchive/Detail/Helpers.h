#pragma once

#include "Types.h"
#include <utility>

namespace detail
{

  inline sf_time_point GetCurrentTimeAsEpoch()
  {
    using sf_clock_type = typename sf_time_point::clock;
    return sf_clock_type::now();
  }

  template<class Stream>
  inline void SeekFile(Stream &stream, u64 Offset);

  /*
   * Copy contents of one file to another up to a max of NumBytes
   * bytes copied (for use in adding/removing files - old archive is copied up to the 
   *   beginning of the directory, the new content added, the directory updated,
   *   the new directory appended to the new file, and the old file deleted)
   **/
  template<class Stream>
  inline void CopyFileContent(Stream &SourceStream, Stream &DstStream, u64 MaxBytesToCopy);
  
  /*
   *  Get the size in bytes for all entities contained in the structure (for the number
   *    of bytes of content in the file, not the size of the structures)
   **/
  class CalculateEntitySize
  {
  public:

    template<class Entity>
    inline CalculateEntitySize& operator()(Entity &&entity)
    {
      GetEntitySize(std::forward<Entity>(entity));
      return *this;
    }

    template<class ...Entities>
    inline CalculateEntitySize& operator()(Entities &&...entities)
    {
      GetEntitySize(std::forward<Entities>(entities)...);
      return *this;
    }

  private:

    template<class Entity>
    inline void GetEntitySize(Entity &&entity)
    {
      EntitySize(std::forward<Entity>(entity));
    }

    template<class Entity, class ...Others>
    inline void GetEntitySize(Entity &&entity, Others &&...others)
    {
      EntitySize(std::forward<Entity>(entity));
      EntitySize(std::forward<Entity>(others)...);
    }

  };

  /*
   *  Construct new archive entity (eg headers, directories, etc)
   **/

  class Construct
  {
  public:

    template<class Entity>
    inline Construct& operator()(Entity &&entity)
    {
      Prepare(std::forward<Entity>(entity));
      return *this;
    }

    template<class ...Entities>
    inline Construct& operator()(Entities &&...entities)
    {
      Prepare(std::forward<Entities>(entities)...);
      return *this;
    }

  private:

    template<class Entity>
    inline void Prepare(Entity &&entity)
    {
      ConstructNew(std::forward<Entity>(entity));
    }

    template<class Entity, class ...Others>
    inline void Prepare(Entity &&entity, Others &&...others)
    {
      Prepare(std::forward<Entity>(entity));
      Prepare(std::forward<Others>(others)...);
    }

  };

}
