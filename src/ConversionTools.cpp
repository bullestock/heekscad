// ConversionTools.cpp
// Copyright (c) 2009, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.
#include "stdafx.h"
#include "ConversionTools.h"
#include "MarkedList.h"
#include "HLine.h"
#include "HArc.h"
#include "HEllipse.h"
#include "HCircle.h"
#include "HSpline.h"
#include "Wire.h"
#include "Face.h"
#include "Edge.h"
#include "Shape.h"
#include "Sketch.h"
#include "Group.h"
#include "HArea.h"

#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>

static MakeToSketch make_to_sketch;
static ConvertSketchesToFace convert_sketches_to_face;
static SketchesArcsToLines sketches_arcs_to_lines;
static CombineSketches combine_sketches;
#ifdef UNITE_SKETCHES
static UniteSketches unite_sketches;
#endif
static GroupSelected group_selected;
static UngroupSelected ungroup_selected;
static MakeEdgesToSketch make_edges_to_sketch;
static TransformToCoordSys transform_to_coordsys;
static ConvertToArea convert_to_area;
static AreaUnion union_area;
static AreaCut cut_area;
static AreaIntersect intersect_area;
static AreaXor xor_area;
static AreaInsideCurves inside_curves;

void GetConversionMenuTools(std::list<Tool*>* t_list){
	// Tools for multiple selected items.
	bool lines_or_arcs_etc_in_marked_list = false;
	int sketches_in_marked_list = 0;
	int areas_in_marked_list = 0;
	bool group_in_marked_list = false;
	bool edges_in_marked_list = false;
	bool ConvertSketchesToFace_added = false;
	CoordinateSystem* first_coord_sys = NULL;
	CoordinateSystem* second_coord_sys = NULL;

	// check to see what types have been marked
	std::list<HeeksObj*>::const_iterator It;
	for(It = wxGetApp().m_marked_list->list().begin(); It != wxGetApp().m_marked_list->list().end(); It++){
		HeeksObj* object = *It;
		switch(object->GetType()){
			case LineType:
			case ArcType:
			case CircleType:
			case EllipseType:
			case SplineType:
				lines_or_arcs_etc_in_marked_list = true;
				break;
			case SketchType:
				sketches_in_marked_list++;
				break;
			case AreaType:
				areas_in_marked_list++;
				break;
			case GroupType:
				group_in_marked_list = true;
				break;
			case EdgeType:
				edges_in_marked_list = true;
				break;
			case CoordinateSystemType:
				if(first_coord_sys == NULL)first_coord_sys = (CoordinateSystem*)object;
				else if(second_coord_sys == NULL)second_coord_sys = (CoordinateSystem*)object;
				break;
		}
	}

	if(lines_or_arcs_etc_in_marked_list || areas_in_marked_list)
	{
		t_list->push_back(&make_to_sketch);
	}

	if(lines_or_arcs_etc_in_marked_list)
	{
		t_list->push_back(&convert_sketches_to_face);
		ConvertSketchesToFace_added = true;
	}

	if(sketches_in_marked_list > 1){
		if(!ConvertSketchesToFace_added)
		{
			t_list->push_back(&convert_sketches_to_face);
			ConvertSketchesToFace_added = true;
		}
		t_list->push_back(&sketches_arcs_to_lines);
		t_list->push_back(&combine_sketches);
#ifdef UNITE_SKETCHES
		t_list->push_back(&unite_sketches);
#endif
	}

	if(areas_in_marked_list > 1)
	{
		t_list->push_back(&union_area);
		t_list->push_back(&cut_area);
		t_list->push_back(&intersect_area);
		t_list->push_back(&xor_area);
		t_list->push_back(&inside_curves);
	}

	if(wxGetApp().m_marked_list->list().size() > 1)t_list->push_back(&group_selected);
	if(group_in_marked_list)t_list->push_back(&ungroup_selected);
	if(edges_in_marked_list)t_list->push_back(&make_edges_to_sketch);
	if(lines_or_arcs_etc_in_marked_list || (sketches_in_marked_list > 0))t_list->push_back(&convert_to_area);

	if(second_coord_sys)
	{
		if(wxGetApp().m_marked_list->list().size() > 2)// more than just two coordinate systems
		{
			transform_to_coordsys.coordsys1 = first_coord_sys;
			transform_to_coordsys.coordsys2 = second_coord_sys;
			t_list->push_back(&transform_to_coordsys);
		}
	}
}


static gp_Pnt GetStart(const TopoDS_Edge &edge)
{
    BRepAdaptor_Curve curve(edge);
    double uStart = curve.FirstParameter();
    gp_Pnt PS;
    gp_Vec VS;
    curve.D1(uStart, PS, VS);

    return(PS);
}

static gp_Pnt GetEnd(const TopoDS_Edge &edge)
{
    BRepAdaptor_Curve curve(edge);
    double uEnd = curve.LastParameter();
    gp_Pnt PE;
    gp_Vec VE;
    curve.D1(uEnd, PE, VE);

    return(PE);
}

struct EdgeComparison
{
    EdgeComparison( const TopoDS_Edge & edge )
    {
        m_reference_edge = edge;
    }

    bool operator()( const TopoDS_Edge & lhs, const TopoDS_Edge & rhs ) const
    {

        std::vector<double> lhs_distances;
        lhs_distances.push_back( GetStart(m_reference_edge).Distance( GetStart(lhs) ) );
        lhs_distances.push_back( GetStart(m_reference_edge).Distance( GetEnd(lhs) ) );
        lhs_distances.push_back( GetEnd(m_reference_edge).Distance( GetStart(lhs) ) );
        lhs_distances.push_back( GetEnd(m_reference_edge).Distance( GetEnd(lhs) ) );
        std::sort(lhs_distances.begin(), lhs_distances.end());

        std::vector<double> rhs_distances;
        rhs_distances.push_back( GetStart(m_reference_edge).Distance( GetStart(rhs) ) );
        rhs_distances.push_back( GetStart(m_reference_edge).Distance( GetEnd(rhs) ) );
        rhs_distances.push_back( GetEnd(m_reference_edge).Distance( GetStart(rhs) ) );
        rhs_distances.push_back( GetEnd(m_reference_edge).Distance( GetEnd(rhs) ) );
        std::sort(rhs_distances.begin(), rhs_distances.end());

        return(*(lhs_distances.begin()) < *(rhs_distances.begin()));
    }

    TopoDS_Edge m_reference_edge;
};

void SortEdges( std::vector<TopoDS_Edge> & edges )
{
	for (std::vector<TopoDS_Edge>::iterator l_itEdge = edges.begin(); l_itEdge != edges.end(); l_itEdge++)
    {
		// We've already begun.  Just sort based on the previous point's location.
		std::vector<TopoDS_Edge>::iterator l_itNextEdge = l_itEdge;
		l_itNextEdge++;

		if (l_itNextEdge != edges.end())
		{
			EdgeComparison compare( *l_itEdge );
			std::sort( l_itNextEdge, edges.end(), compare );
		} // End if - then
    } // End for
} // End SortEdges() method



bool ConvertLineArcsToWire2(const std::list<HeeksObj *> &list, TopoDS_Wire &wire)
{
	std::vector<TopoDS_Edge> edges;
	std::list<HeeksObj*> list2;
	std::list<HeeksObj*>::const_iterator It;
	for(It = list.begin(); It != list.end(); It++){
		HeeksObj* object = *It;
		if(object->GetType() == SketchType){
			for(HeeksObj* child = object->GetFirstChild(); child; child = object->GetNextChild())
			{
				list2.push_back(child);
			}
		}
		else{
			list2.push_back(object);
		}
	}

	for(std::list<HeeksObj*>::iterator It = list2.begin(); It != list2.end(); It++){
		HeeksObj* object = *It;
		switch(object->GetType()){
			case LineType:
				{
					HLine* line = (HLine*)object;
					edges.push_back(BRepBuilderAPI_MakeEdge(line->A, line->B));
				}
				break;
			case ArcType:
				{
					HArc* arc = (HArc*)object;
					edges.push_back(BRepBuilderAPI_MakeEdge(arc->GetCircle(), arc->A, arc->B));
				}
				break;
			case SplineType:
				{
					HSpline* spline = (HSpline*)object;
					edges.push_back(BRepBuilderAPI_MakeEdge(spline->m_spline));
				}
				break;
		}
	}

	if(edges.size() > 0){
		wire = EdgesToWire(edges);
		return true;
	}

	return false;
}


bool ConvertSketchToEdges(HeeksObj *object, std::list< std::vector<TopoDS_Edge> > &edges)
{
    std::list<HeeksObj*> line_arc_list;

	if(object->GetType() == SketchType)
	{
		std::list<HeeksObj*> new_separate_sketches;
		((CSketch*)object)->ExtractSeparateSketches(new_separate_sketches, false);
		if(new_separate_sketches.size() > 1)
		{
			// call again with each separate sketch
			for(std::list<HeeksObj*>::iterator It = new_separate_sketches.begin(); It != new_separate_sketches.end(); It++)
			{
				HeeksObj* sketch = *It;
				if(!ConvertSketchToEdges(sketch, edges))return false;
			}
			return true;
		}
		else
		{
			for(HeeksObj* child = object->GetFirstChild(); child; child = object->GetNextChild())
			{
				line_arc_list.push_back(child);
			}
			std::vector<TopoDS_Edge> empty_list;
			edges.push_back(empty_list);
		}
	}
	else
	{
		line_arc_list.push_back(object);
	}

	if(edges.size() == 0)
	{
		std::vector<TopoDS_Edge> empty_list;
		edges.push_back(empty_list);
	}

	const double max_tolerance = 10.0;
	for(std::list<HeeksObj*>::const_iterator It = line_arc_list.begin(); It != line_arc_list.end(); It++){
	    try {
            HeeksObj* object = *It;
            switch(object->GetType()){
                case LineType:
                    {
						bool done = false;
						double tolerance = wxGetApp().m_geom_tol;
						while ((! done) && (tolerance < max_tolerance))
						{
							HLine* line = (HLine*)object;
							if(!(line->A.IsEqual(line->B, wxGetApp().m_geom_tol)))
							{
								BRep_Builder aBuilder;
								TopoDS_Vertex start, end;

								aBuilder.MakeVertex (start, line->A, wxGetApp().m_geom_tol);
								start.Orientation (TopAbs_REVERSED);

								aBuilder.MakeVertex (end, line->B, wxGetApp().m_geom_tol);
								end.Orientation (TopAbs_FORWARD);

								BRepBuilderAPI_MakeEdge edge(start, end);
								if (! edge.IsDone())
								{	// return(false);
									tolerance *= 10.0;
								}
								else
								{
									edges.back().push_back(edge.Edge());
									done = true;
								}
							}
							else
							{
								break;
							}

							if (! done)
							{
								return(false);
							}
						} // End while
                    }
                    break;
                case ArcType:
                    {
						bool done = false;
						double tolerance = wxGetApp().m_geom_tol;
						while ((! done) && (tolerance < max_tolerance))
						{
							HArc* arc = (HArc*)object;

							BRep_Builder aBuilder;
							TopoDS_Vertex start, end;

							aBuilder.MakeVertex (start, arc->A, wxGetApp().m_geom_tol);
							start.Orientation (TopAbs_REVERSED);

							aBuilder.MakeVertex (end, arc->B, wxGetApp().m_geom_tol);
							end.Orientation (TopAbs_FORWARD);

							BRepBuilderAPI_MakeEdge edge(arc->GetCircle(), start, end);
							if (! edge.IsDone())
							{
								// return(false);
								tolerance *= 10.0;
							}
							else
							{
								edges.back().push_back(edge.Edge());
								done = true;
							}
						} // End while

						if (! done)
						{
							return(false);
						}
                    }
                    break;
                case CircleType:
                    {
                        HCircle* circle = (HCircle*)object;
                        edges.back().push_back(BRepBuilderAPI_MakeEdge(circle->GetCircle()));
                    }
                    break;
                case EllipseType:
                    {
                        HEllipse* ellipse = (HEllipse*)object;
                        edges.back().push_back(BRepBuilderAPI_MakeEdge(ellipse->GetEllipse()));
                    }
                    break;
                case SplineType:
                    {
                        HSpline* spline = (HSpline*)object;
                        edges.back().push_back(BRepBuilderAPI_MakeEdge(spline->m_spline));
                    }
                    break;

				default:
					{
						wxString message;
						message << _("Cannot convert object type ") << wxGetApp().HeeksType(object->GetType()) << _(" to edge");
						wxMessageBox(message);
						return(false);
					}
            }
	    } // End try
	    catch (const Standard_Failure& e)
	    {
			wxMessageBox(wxString(_("Error converting sketch to face")) + _T(": ") + Ctt(e.GetMessageString()));
			return false;
	    }
	}

    return(true);
}

TopoDS_Wire EdgesToWire(const std::vector<TopoDS_Edge> &edges)
{
	BRepBuilderAPI_MakeWire wire_maker;
	for(std::vector<TopoDS_Edge>::const_iterator It = edges.begin(); It != edges.end(); It++)
	{
		const TopoDS_Edge &edge = *It;
		wire_maker.Add(edge);
	}

	return wire_maker.Wire();
}

bool SketchToWires(HeeksObj* sketch, std::list<TopoDS_Wire> &wire_list)
{
    std::list< std::vector<TopoDS_Edge> > edges_list;

    if (!ConvertSketchToEdges(sketch, edges_list))
        return false;

    for (const auto& edges : edges_list)
    {
        wire_list.push_back(EdgesToWire(edges));
    }
    return true;
}

bool ConvertEdgesToFaceOrWire(const std::vector<TopoDS_Edge> &edges, std::list<TopoDS_Shape> &face_or_wire, bool face_not_wire)
{
	// It's not enough to add the edges to the wire in an arbitrary order.  If the adjacent edges
	// don't connect then the wire ends up losing one of the edges.  We must sort the edge objects
	// so that they're connected (or best we can) before constructing the TopoDS_Wire object from
	// them.
	// So, please call SortEdges before getting to here.
	try
	{
		TopoDS_Wire wire = EdgesToWire(edges);

		if(face_not_wire)
		{
			BRepBuilderAPI_MakeFace make_face(wire);
			if(make_face.IsDone() == Standard_False)
                face_or_wire.push_back(wire);
			else
                face_or_wire.push_back(make_face.Face());
		}
		else
		{
			face_or_wire.push_back(wire);
		}
	}
	catch (const Standard_Failure& e)
    {
		wxMessageBox(wxString(_("Error converting sketch to face")) + _T(": ") + Ctt(e.GetMessageString()));
		return false;
	}
	catch (...)
	{
		wxMessageBox(_("Fatal Error converting sketch to face"));
		return false;
	}

	return true;
}

bool ConvertSketchToFaceOrWire(HeeksObj* object, std::list<TopoDS_Shape> &face_or_wire, bool face_not_wire)
{
    std::list< std::vector<TopoDS_Edge> > edges;

    if (! ConvertSketchToEdges(object, edges))
    {
        return(false);
    }

	for(std::list< std::vector<TopoDS_Edge> >::iterator It = edges.begin(); It != edges.end(); It++)
	{
		std::vector<TopoDS_Edge> &list = *It;
		if(list.size() > 0)
		{
			SortEdges(list);
			if(!ConvertEdgesToFaceOrWire(list, face_or_wire, face_not_wire))return false;
		}
	}

	return true;
}

bool ConvertFaceToSketch2(const TopoDS_Face& face, HeeksObj* sketch, double deviation)
{
	// given a face, this adds lines and arcs to the given sketch
	// loop through all the loops
	TopoDS_Wire outerWire=BRepTools::OuterWire(face);

	for (TopExp_Explorer expWire(face, TopAbs_WIRE); expWire.More(); expWire.Next())
	{
		const TopoDS_Shape &W = expWire.Current();
		for(BRepTools_WireExplorer expEdge(TopoDS::Wire(W)); expEdge.More(); expEdge.Next())
		{
			const TopoDS_Shape &E = expEdge.Current();
			if(!ConvertEdgeToSketch2(TopoDS::Edge(E), sketch, deviation))return false;
		}
	}

	return true; // success
}


bool ConvertWireToSketch(const TopoDS_Wire& wire, HeeksObj* sketch, double deviation)
{
    const TopoDS_Shape &W = wire;
    for(BRepTools_WireExplorer expEdge(TopoDS::Wire(W)); expEdge.More(); expEdge.Next())
    {
        const TopoDS_Shape &E = expEdge.Current();
        if(!ConvertEdgeToSketch2(TopoDS::Edge(E), sketch, deviation))return false;
    }

	return true; // success
}



bool ConvertEdgeToSketch2(const TopoDS_Edge& edge, HeeksObj* sketch, double deviation, bool reverse)
{
	// enum GeomAbs_CurveType
	// 0 - GeomAbs_Line
	// 1 - GeomAbs_Circle
	// 2 - GeomAbs_Ellipse
	// 3 - GeomAbs_Hyperbola
	// 4 - GeomAbs_Parabola
	// 5 - GeomAbs_BezierCurve
	// 6 - GeomAbs_BSplineCurve
	// 7 - GeomAbs_OtherCurve

	BRepAdaptor_Curve curve(edge);
	GeomAbs_CurveType curve_type = curve.GetType();
	bool sense = (edge.Orientation() == TopAbs_FORWARD);

	if(reverse)sense = !sense;

	switch(curve_type)
	{
		case GeomAbs_Line:
			// make a line
		{
			double uStart = curve.FirstParameter();
			double uEnd = curve.LastParameter();
			gp_Pnt PS;
			gp_Vec VS;
			curve.D1(uStart, PS, VS);
			gp_Pnt PE;
			gp_Vec VE;
			curve.D1(uEnd, PE, VE);
			HLine* new_object = new HLine(sense ? PS:PE, sense ?PE:PS, &wxGetApp().current_color);
			sketch->Add(new_object, NULL);
		}
		break;

		case GeomAbs_Circle:
			// make an arc
		{
			double uStart = curve.FirstParameter();
			double uEnd = curve.LastParameter();
			gp_Pnt PS;
			gp_Vec VS;
			curve.D1(uStart, PS, VS);
			gp_Pnt PE;
			gp_Vec VE;
			curve.D1(uEnd, PE, VE);
			gp_Circ circle = curve.Circle();
			gp_Ax1 axis = circle.Axis();
			if(!sense)
			{
				axis.SetDirection(-axis.Direction());
				circle.SetAxis(axis);
			}

			if(curve.IsPeriodic())
			{
				double period = curve.Period();
				double uHalf = uStart + period/2;
				gp_Pnt PH;
				gp_Vec VH;
				curve.D1(uHalf, PH, VH);
				{
					HArc* new_object = new HArc(PS, PH, circle, &wxGetApp().current_color);
					sketch->Add(new_object, NULL);
				}
				{
					HArc* new_object = new HArc(PH, PE, circle, &wxGetApp().current_color);
					sketch->Add(new_object, NULL);
				}
			}
			else
			{
				HArc* new_object = new HArc(sense ? PS:PE, sense ?PE:PS, circle, &wxGetApp().current_color);
				sketch->Add(new_object, NULL);
			}
		}
		break;

		case GeomAbs_BSplineCurve:
			{
				std::list<HeeksObj*> new_spans;
				HSpline::ToBiarcs(curve.BSpline(), new_spans, deviation, curve.FirstParameter(), curve.LastParameter());
				if(sense)
				{
					for(std::list<HeeksObj*>::iterator It = new_spans.begin(); It != new_spans.end(); It++)
						sketch->Add(*It, NULL);
				}
				else
				{
					for(std::list<HeeksObj*>::reverse_iterator It = new_spans.rbegin(); It != new_spans.rend(); It++)
					{
						HeeksObj* object = *It;
						CSketch::ReverseObject(object);
						sketch->Add(object, NULL);
					}
				}
			}
			break;


		default:
		{
			// make lots of small lines
			BRepTools::Clean(edge);
			BRepMesh_IncrementalMesh(edge, deviation);

			TopLoc_Location L;
			Handle(Poly_Polygon3D) Polyg = BRep_Tool::Polygon3D(edge, L);
			if (!Polyg.IsNull()) {
				const TColgp_Array1OfPnt& Points = Polyg->Nodes();
				Standard_Integer po;
				gp_Pnt prev_p;
				int i = 0;
				for (po = sense ? Points.Lower():Points.Upper(); (sense && (po <= Points.Upper())) || (!sense && (po >= Points.Lower())); i++) {
					gp_Pnt p = (Points.Value(po)).Transformed(L);
					if(i != 0)
					{
						HLine* new_object = new HLine(prev_p, p, &wxGetApp().current_color);
						sketch->Add(new_object, NULL);
					}
					prev_p = p;
					if(sense)po++;
					else po--;
				}
			}
		}
		break;
	}

	return true;
}

void ConvertSketchesToFace::Run()
{
	std::list< std::vector<TopoDS_Edge> > individual_edges;
	std::list<HeeksObj*>::const_iterator It;
	for(It = wxGetApp().m_marked_list->list().begin(); It != wxGetApp().m_marked_list->list().end(); It++){
		HeeksObj* object = *It;
		switch(object->GetType())
		{
		case SketchType:
			{
				std::list<TopoDS_Shape> faces;
				if(ConvertSketchToFaceOrWire(object, faces, true))
				{
					for(std::list<TopoDS_Shape>::iterator It2 = faces.begin(); It2 != faces.end(); It2++)
					{
						TopoDS_Shape& face = *It2;
						wxGetApp().AddUndoably(new CFace(TopoDS::Face(face)), NULL, NULL);
					}
				}
			}
			break;

		case LineType:
		case ArcType:
		case CircleType:
		case EllipseType:
		case SplineType:
			{
				if (! ConvertSketchToEdges(object, individual_edges))return;
			}
			break;
		}
	}

	for(std::list< std::vector<TopoDS_Edge> >::iterator It = individual_edges.begin(); It != individual_edges.end(); It++)
	{
		std::vector<TopoDS_Edge> &edges = *It;
		if(edges.size() > 0)
		{
			SortEdges(edges);
			std::list<TopoDS_Shape> faces;
			if(ConvertEdgesToFaceOrWire(edges, faces, true))
			{
				for(std::list<TopoDS_Shape>::iterator It2 = faces.begin(); It2 != faces.end(); It2++)
				{
					TopoDS_Shape& face = *It2;
					wxGetApp().AddUndoably(new CFace(TopoDS::Face(face)), NULL, NULL);
				}
			}
		}
	}
}

static void AddLineOrArc(CSketch* sketch, Span &span)
{
	if(span.m_v.m_type == 0)
	{
		HLine* new_object = new HLine(gp_Pnt(span.m_p.x, span.m_p.y, 0), gp_Pnt(span.m_v.m_p.x, span.m_v.m_p.y, 0), &wxGetApp().current_color);
		sketch->Add(new_object, NULL);
	}
	else
	{
		gp_Dir axis = (span.m_v.m_type > 0) ? gp_Dir(0, 0, 1):gp_Dir(0, 0, -1);
		double radius = span.m_p.dist(span.m_v.m_c);
		gp_Circ c(gp_Ax2(gp_Pnt(span.m_v.m_c.x, span.m_v.m_c.y, 0), axis), radius);
		HArc* new_object = new HArc(gp_Pnt(span.m_p.x, span.m_p.y, 0), gp_Pnt(span.m_v.m_p.x, span.m_v.m_p.y, 0), c, &wxGetApp().current_color);
		sketch->Add(new_object, NULL);
	}
}

static void AddLinesOrArcs(CSketch* sketch, CArea &area)
{
	for(std::list<CCurve>::iterator It2 = area.m_curves.begin(); It2 != area.m_curves.end(); It2++)
	{
		CCurve& curve = *It2;
		std::list<Span> spans;
		curve.GetSpans(spans);
		for(std::list<Span>::iterator It3 = spans.begin(); It3 != spans.end(); It3++)
		{
			Span& span = *It3;
			AddLineOrArc(sketch, span);
		}
	}
}

CSketch* MakeNewSketchFromArea(CArea &area)
{
	CSketch* sketch = new CSketch();
	AddLinesOrArcs(sketch, area);
	return sketch;
}

void MakeToSketch::Run(){
	std::list<HeeksObj*> objects_to_delete;

	CSketch* sketch = new CSketch();

	std::list<HeeksObj*>::const_iterator It;
	for(It = wxGetApp().m_marked_list->list().begin(); It != wxGetApp().m_marked_list->list().end(); It++){
		HeeksObj* object = *It;
		switch(object->GetType()){
			case LineType:
			case ArcType:
			case CircleType:
			case EllipseType:
			case SplineType:
				{
					HeeksObj* new_object = object->MakeACopy();
					objects_to_delete.push_back(object);
					sketch->Add(new_object, NULL);
				}
				break;
			case AreaType:
				{
					AddLinesOrArcs(sketch, ((HArea*)object)->m_area);
				}
				break;
			default:
				break;
		}
	}

	wxGetApp().Add(sketch, NULL);
	wxGetApp().Remove(objects_to_delete);
}

void MakeEdgesToSketch::Run(){
	CSketch* sketch = new CSketch();

	std::list<HeeksObj*>::const_iterator It;
	for(It = wxGetApp().m_marked_list->list().begin(); It != wxGetApp().m_marked_list->list().end(); It++){
		HeeksObj* object = *It;
		switch(object->GetType()){
			case EdgeType:
				{
					ConvertEdgeToSketch2(((CEdge*)object)->Edge(), sketch, FaceToSketchTool::deviation);
				}
				break;
			default:
				break;
		}
	}

	wxGetApp().Add(sketch, NULL);
}

void TransformToCoordSys::Run(){
	double m[16];
	extract(coordsys2->GetMatrix() * (coordsys1->GetMatrix().Inverted()), m);

	// move any selected objects
	for(std::list<HeeksObj *>::iterator It = wxGetApp().m_marked_list->list().begin(); It != wxGetApp().m_marked_list->list().end(); It++)
	{
		HeeksObj* object = *It;
		if((object == coordsys1) || (object == coordsys2))continue;
		wxGetApp().TransformUndoably(object, m);
	}
}

static CSketch* sketch_for_arcs_to_lines = NULL;
static bool point_found_for_arc_to_lines = false;
static double point_for_arcs_to_lines[3];
static void callback_for_arcs_to_lines(const double* p)
{
	if(point_found_for_arc_to_lines)
	{
		HLine* new_object = new HLine(make_point(p), make_point(point_for_arcs_to_lines), &wxGetApp().current_color);
		sketch_for_arcs_to_lines->Add(new_object, NULL);
	}
	point_found_for_arc_to_lines = true;
	memcpy(point_for_arcs_to_lines, p, 3*sizeof(double));
}

HeeksObj* SplitArcsIntoLittleLines(HeeksObj* sketch)
{
	CSketch* new_sketch = new CSketch;
	sketch_for_arcs_to_lines = new_sketch;
	point_found_for_arc_to_lines = false;
	for(HeeksObj* o = sketch->GetFirstChild(); o; o = sketch->GetNextChild())
	{
		//reset state machine
		point_found_for_arc_to_lines = false;
		o->GetSegments(callback_for_arcs_to_lines, 0.2/FaceToSketchTool::deviation);
	}
	return new_sketch;
}

void SketchesArcsToLines::Run(){
	std::list<HeeksObj*> copy_of_marked_list = wxGetApp().m_marked_list->list();
	std::list<HeeksObj*> objects_to_delete;
	std::list<HeeksObj*> new_objects;
	for(std::list<HeeksObj*>::const_iterator It = copy_of_marked_list.begin(); It != copy_of_marked_list.end(); It++){
		HeeksObj* object = *It;
		if(object->GetType() == SketchType){
			HeeksObj* new_object = SplitArcsIntoLittleLines(object);
			new_objects.push_back(new_object);
			objects_to_delete.push_back(object);
		}
	}

	wxGetApp().Remove(objects_to_delete);
}

static CArea* area_to_add_to = NULL;
static CCurve* curve_to_add_to = NULL;

static void MakeNewCurveIfNecessary(HeeksObj* object)
{
	if(curve_to_add_to)
	{
		// does this object join the end of the current curve
		double s[3];
		if(!object->GetStartPoint(s) || (Point(s[0], s[1]) != curve_to_add_to->m_vertices.back().m_p))
		{
			// it doesn't join. we need a new curve
			curve_to_add_to = NULL;
		}
	}

	if(curve_to_add_to == NULL)
	{
		// make a new curve
		area_to_add_to->m_curves.push_back(CCurve());
		curve_to_add_to = &(area_to_add_to->m_curves.back());

		// add the start point of this object
		double s[3];
		if(object->GetStartPoint(s))
		{
			curve_to_add_to->append(Point(s[0], s[1]));
		}
	}
}

static void AddObjectToArea(HeeksObj* object)
{
	switch(object->GetType())
	{
	case SketchType:
		{
			for(HeeksObj* child = ((CSketch*)object)->GetFirstChild(); child; child = ((CSketch*)object)->GetNextChild())
			{
				AddObjectToArea(child);
			}
		}
		break;

	case LineType:
		{
			MakeNewCurveIfNecessary(object);
			double e[3];
			if(object->GetEndPoint(e))curve_to_add_to->append(Point(e[0], e[1]));
		}
		break;
	case ArcType:
		{
			MakeNewCurveIfNecessary(object);
			double e[3], c[3];
			if(object->GetEndPoint(e) && object->GetCentrePoint(c))
			{
				int dir = (((HArc*)object)->m_axis.Direction().Z() > 0.0) ? 1:-1;
				curve_to_add_to->append(CVertex(dir, Point(e[0], e[1]), Point(c[0], c[1])));
			}
		}
		break;
	case CircleType:
		{
			// add a couple of arcs
			double c[3];
			if(object->GetCentrePoint(c))
			{
				MakeNewCurveIfNecessary(object);
				Point pc(c[0], c[1]);
				double radius = ((HCircle*)object)->GetCircle().Radius();
				Point p0 = pc + Point(radius, 0.0);
				Point p1 = pc + Point(-radius, 0.0);
				curve_to_add_to->append(p0);
				curve_to_add_to->append(CVertex(1, p1, pc));
				curve_to_add_to->append(CVertex(1, p0, pc));
			}
		}

		break;
	case EllipseType:
		// to do - add some arcs
		break;
	case SplineType:
		// to do - add some arcs
		break;
	}
}

void ConvertToArea::Run(){
	std::list<HeeksObj*> copy_of_marked_list = wxGetApp().m_marked_list->list();
	std::list<HeeksObj*> objects_to_delete;

	CArea area;
	area_to_add_to = &area;
	curve_to_add_to = NULL;

	for(std::list<HeeksObj*>::const_iterator It = copy_of_marked_list.begin(); It != copy_of_marked_list.end(); It++){
		HeeksObj* object = *It;
		AddObjectToArea(object);
		objects_to_delete.push_back(object);
	}

	HArea* new_object = new HArea(area);
	wxGetApp().AddUndoably(new_object, NULL, NULL);

	wxGetApp().DeleteUndoably(objects_to_delete);
}

static void AreaToolRun(int type)
{
	std::list<HeeksObj*> copy_of_marked_list = wxGetApp().m_marked_list->list();
	std::list<HeeksObj*> objects_to_delete;

	CArea area;
	bool area_found = false;

	for(std::list<HeeksObj*>::const_iterator It = copy_of_marked_list.begin(); It != copy_of_marked_list.end(); It++){
		HeeksObj* object = *It;
		if(object->GetType() == AreaType)
		{
			if(area_found)
			{
				switch(type)
				{
				case 1:
					area.Union(((HArea*)object)->m_area);
					break;
				case 2:
					area.Subtract(((HArea*)object)->m_area);
					break;
				case 3:
					area.Intersect(((HArea*)object)->m_area);
					break;
				case 4:
					area.Xor(((HArea*)object)->m_area);
					break;
				case 5:
					std::list<CCurve> curves;
					area.InsideCurves(((HArea*)object)->m_area.m_curves.front(), curves);
					area = CArea();
					for(std::list<CCurve>::iterator It = curves.begin(); It != curves.end(); It++)
					{
						area.append(*It);
					}
					break;
				}
			}
			else
			{
				area = ((HArea*)object)->m_area;
				area_found = true;
			}
			objects_to_delete.push_back(object);
		}
	}

	HArea* new_object = new HArea(area);
	wxGetApp().AddUndoably(new_object, NULL, NULL);

	wxGetApp().DeleteUndoably(objects_to_delete);
}

void AreaUnion::Run(){
	AreaToolRun(1);
}

void AreaCut::Run(){
	AreaToolRun(2);
}

void AreaIntersect::Run(){
	AreaToolRun(3);
}

void AreaXor::Run(){
	AreaToolRun(4);
}

void AreaInsideCurves::Run(){
	AreaToolRun(5);
}

void CombineSketches::Run(){
	CSketch* sketch1 = NULL;
	std::list<HeeksObj*>::const_iterator It;
	std::list<HeeksObj*> copy_of_marked_list = wxGetApp().m_marked_list->list();

	for(It = copy_of_marked_list.begin(); It != copy_of_marked_list.end(); It++){
		HeeksObj* object = *It;
		if(object->GetType() == SketchType){
			if(sketch1)
			{
				std::list<HeeksObj*> new_lines_and_arcs;
				for(HeeksObj* o = object->GetFirstChild(); o; o = object->GetNextChild())
				{
					new_lines_and_arcs.push_back(o);
				}
				wxGetApp().DeleteUndoably(object);
				wxGetApp().AddUndoably(new_lines_and_arcs, sketch1);
			}
			else
			{
				sketch1 = (CSketch*)object;
			}
		}
	}

	if(sketch1)sketch1->ReOrderSketch(SketchOrderTypeReOrder);
}

void GroupSelected::Run(){
	if(wxGetApp().m_marked_list->list().size() < 2)
	{
		return;
	}

	CGroup* new_group = new CGroup;
	std::list<HeeksObj*> copy_of_marked_list = wxGetApp().m_marked_list->list();

	wxGetApp().Remove(copy_of_marked_list);

	std::list<HeeksObj*>::iterator it;
	for(it = copy_of_marked_list.begin(); it != copy_of_marked_list.end(); it++)
		(*it)->m_owner = NULL;

	new_group->Add(copy_of_marked_list);
	wxGetApp().Add(new_group, NULL);
	wxGetApp().m_marked_list->Clear(true);
	wxGetApp().Repaint();
}

void UngroupSelected::Run(){
	if(wxGetApp().m_marked_list->list().size() == 0)return;

	std::list<HeeksObj*> copy_of_marked_list = wxGetApp().m_marked_list->list();
	for(std::list<HeeksObj*>::const_iterator It = copy_of_marked_list.begin(); It != copy_of_marked_list.end(); It++){
		HeeksObj* object = *It;
		if(object->GetType() == GroupType)
		{
			std::list<HeeksObj*> list;
			for(HeeksObj* o = ((CGroup*)object)->GetFirstChild(); o; o = ((CGroup*)object)->GetNextChild())
			{
				list.push_back(o);
			}
			wxGetApp().Remove(list);
			((ObjList*)&wxGetApp())->Add(list);
			wxGetApp().Remove(object);
		}
	}

	wxGetApp().m_marked_list->Clear(true);
	wxGetApp().Repaint();
}
