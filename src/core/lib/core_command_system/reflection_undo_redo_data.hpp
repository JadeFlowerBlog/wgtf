#ifndef REFLECTION_UNDO_REDO_DATA_HPP
#define REFLECTION_UNDO_REDO_DATA_HPP

#include "undo_redo_data.hpp"
#include "core_serialization/resizing_memory_stream.hpp"
#include "core_reflection_utils/commands/reflectedproperty_undoredo_helper.hpp"

namespace wgt
{
class CommandInstance;
class PropertyAccessorListener;
class BinaryBlock;

class ReflectionUndoRedoData : public UndoRedoData
{
public:
	friend CommandInstance;

	ReflectionUndoRedoData( CommandInstance & commandInstance );

	void connect();
	void disconnect();
	void consolidate();

	void undo() override;
	void redo() override;

	ObjectHandle getCommandDescription() const;

    std::shared_ptr<BinaryBlock> getUndoData() const;
    std::shared_ptr<BinaryBlock> getRedoData() const;
    void setUndoData( const std::shared_ptr<BinaryBlock> & undoData );
    void setRedoData( const std::shared_ptr<BinaryBlock> & redoData );

private:
	CommandInstance &			commandInstance_;
	ResizingMemoryStream		undoData_;
	ResizingMemoryStream		redoData_;
	std::shared_ptr< PropertyAccessorListener > paListener_;
	ReflectedPropertyUndoRedoUtility::UndoRedoHelperList	undoRedoHelperList_;
};
} // end namespace wgt
#endif
